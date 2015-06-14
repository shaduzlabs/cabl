/*----------------------------------------------------------------------------------------------------------------------   

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include "devices/DeviceMaschineMK1.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions_SL.h"
#include "Macros.h"

#include "gfx/displays/GDisplayMaschineMK1.h"

#include <thread>


//\todo delete debug includes
#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------------------------------------------------

namespace
{
  static const uint16_t kMASMK1_vendorId                      = 0x17CC;
  static const uint16_t kMASMK1_productId                     = 0x0808;
  static const uint8_t  kMASMK1_endpointDisplay               = 0x08;
  static const uint8_t  kMASMK1_endpointOut                   = 0x01;
  static const uint8_t  kMASMK1_endpointInputPads             = 0x84;
  static const uint8_t  kMASMK1_endpointInputButtonsAndDials  = 0x81;
  
  static const uint8_t  kMASMK1_defaultDisplaysBacklight      = 0x5C;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

enum class DeviceMaschineMK1::Led : uint8_t
{
  Pad4,
  Pad3,
  Pad2,
  Pad1,
  Pad8,
  Pad7,
  Pad6,
  Pad5,
  Pad12,
  Pad11,
  Pad10,
  Pad9,
  Pad16,
  Pad15,
  Pad14,
  Pad13,
  Mute,
  Solo,
  Select,
  Duplicate,
  Navigate,
  Keyboard,
  Pattern,
  Scene,
  Shift,
  Erase,
  Grid,
  TransportRight,
  Rec,
  Play,
  Unused1,
  TransportLeft,
  Loop,
  GroupH,
  GroupG,
  GroupD,
  GroupC,
  GroupF,
  GroupE,
  GroupB,
  GroupA,
  AutoWrite,
  Snap,
  BrowseRight,
  BrowseLeft,
  Sampling,
  Browse,
  Step,
  Control,
  DisplayButton8,
  DisplayButton7,
  DisplayButton6,
  DisplayButton5,
  DisplayButton4,
  DisplayButton3,
  DisplayButton2,
  DisplayButton1,
  NoteRepeat,
  DisplayBacklight,
  Unused2,
  Unused3,
  Unused4,
  Unknown,
};

//----------------------------------------------------------------------------------------------------------------------

enum class DeviceMaschineMK1::Button : uint8_t
{
  Mute,
  Solo,
  Select,
  Duplicate,
  Navigate,
  Keyboard,
  Pattern,
  Scene,

  Rec = 9,
  Erase,
  Shift,
  Grid,
  TransportRight,
  TransportLeft,
  Loop,

  GroupE,
  GroupF,
  GroupG,
  GroupH,
  GroupD,
  GroupC,
  GroupB,
  GroupA,

  Control,
  Browse,
  BrowseLeft,
  Snap,
  AutoWrite,
  BrowseRight,
  Sampling,
  Step,

  DisplayButton8,
  DisplayButton7,
  DisplayButton6,
  DisplayButton5,
  DisplayButton4,
  DisplayButton3,
  DisplayButton2,
  DisplayButton1,

  NoteRepeat,
  Play,
};

//----------------------------------------------------------------------------------------------------------------------

enum class DeviceMaschineMK1::Encoder : uint8_t
{
  Encoder8,
  Encoder4,
  Swing,
  Encoder7,
  Encoder3,
  Tempo,
  Encoder6,
  Encoder2,
  Volume,
  Encoder5,
  Encoder1,
};

//----------------------------------------------------------------------------------------------------------------------

DeviceMaschineMK1::DeviceMaschineMK1(tPtr<DeviceHandle> pDeviceHandle_)
  : Device(std::move(pDeviceHandle_))
  , m_encodersInitialized(false)
{
  m_displays[0].reset( new GDisplayMaschineMK1 );
  m_displays[1].reset( new GDisplayMaschineMK1 );
  m_leds.resize(kMASMK1_ledsDataSize);
}

//----------------------------------------------------------------------------------------------------------------------
  
DeviceMaschineMK1::~DeviceMaschineMK1()
{

}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::setLed( Device::Button btn_, uint8_t val_)
{
  Led led = getLed( btn_ );
  
  if( Led::Unknown != led )
  {
    uint8_t currentVal = m_leds[ static_cast<uint16_t>(led)];
    
    m_leds[ static_cast<uint16_t>(led)] = val_;
    if( led > Led::Unused1 )
    {
      m_isDirtyLedGroup1 = ( val_ != currentVal );
    }
    else
    {
      m_isDirtyLedGroup0 = ( val_ != currentVal );
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::setLed( Device::Button btn_, uint8_t r_, uint8_t g_, uint8_t b_)
{
  Led led = getLed( btn_ );
  
  if( Led::Unknown != led )
  {
    uint8_t currentVal = m_leds[ static_cast<uint16_t>(led)];

    // Use "Maximum decomposition" -> take the channel with the highest value
    uint8_t newVal = util::max<uint8_t>( r_, g_, b_ );

    m_leds[ static_cast<uint16_t>(led)] = newVal;
    if( led > Led::Unused1 )
    {
      m_isDirtyLedGroup1 = ( currentVal != newVal );
    }
    else
    {
      m_isDirtyLedGroup0 = ( currentVal != newVal );
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay* DeviceMaschineMK1::getDisplay( uint8_t displayIndex_ )
{
  if( displayIndex_ > 1 )
  {
    return nullptr;
  }
  
  return m_displays[displayIndex_].get();
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::tick()
{
  for( int i=0; i< kMASMK1_nDisplays; i++ )
  {
    //\todo enable once display dirty flag is properly set
    //    if( m_displays[i]->isDirty() )
    {
      sendFrame(i);
    }
  }
  if (!read())
  {
    return false;
  }

  if( m_isDirtyLedGroup0 || m_isDirtyLedGroup1 )
  {
    if(!sendLeds())
    {
      return false;
    }
  }

  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::init()
{

  getDeviceHandle()->write(Transfer({ 0x01 }), kMASMK1_endpointOut);

  // Displays
  for( int i=0; i< kMASMK1_nDisplays; i++ )
  {
    initDisplay(i);
    m_displays[i].get()->white();
  }
  
  getDeviceHandle()->write(Transfer({ 0x0B, 0xFF, 0x02, 0x05 }), kMASMK1_endpointOut);

  // Leds
  std::fill(m_leds.begin(), m_leds.end(), 0);
  m_leds[ static_cast<uint8_t>( Led::DisplayBacklight )] = kMASMK1_defaultDisplaysBacklight;
  m_isDirtyLedGroup0 = true;
  m_isDirtyLedGroup1 = true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::initDisplay( uint8_t displayIndex_ )
{
  if( displayIndex_ > 1 )
  {
    return;
  }
  
  uint8_t displayNumber = displayIndex_ << 1;
  
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x30                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x04, 0xCA, 0x04, 0x0F, 0x00 }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x02, 0xBB, 0x00             }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0xD1                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x94                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x81, 0x1E, 0x02       }), kMASMK1_endpointDisplay );
  
  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x02, 0x20, 0x08             }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x02, 0x20, 0x0B             }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0xA6                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x31                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x04, 0x32, 0x00, 0x00, 0x05 }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x34                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x30                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x04, 0xBC, 0x00, 0x01, 0x02 }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x75, 0x00, 0x3F       }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x15, 0x00, 0x54       }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x5C                   }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0x25                   }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0xAF                   }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x04, 0xBC, 0x02, 0x01, 0x01 }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x01, 0xA6                   }), kMASMK1_endpointDisplay );
  
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x81, 0x25, 0x02       }), kMASMK1_endpointDisplay );
//  buf[i++] = 0x09;//contrast & 0x3f;
//  buf[i++] = 0x04;//(contrast >> 6) & 0x7;

}

//----------------------------------------------------------------------------------------------------------------------
  
bool DeviceMaschineMK1::sendFrame( uint8_t displayIndex_ )
{
  if( displayIndex_ > 1 )
  {
    return false;
  }
  
  uint8_t displayNumber = displayIndex_ << 1;
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x75, 0x00, 0x3F }), kMASMK1_endpointDisplay );
  getDeviceHandle()->write( Transfer({ displayNumber, 0x00, 0x03, 0x15, 0x00, 0x54 }), kMASMK1_endpointDisplay );
  
  uint16_t offset = 0;
  const uint16_t dataSize = 502;
  
  if(!getDeviceHandle()->write(
      Transfer({ displayNumber, 0x01, 0xF7, 0x5C },
        m_displays[displayIndex_]->getPtr( offset ),
        dataSize
      ),
      kMASMK1_endpointDisplay
  ))
  {
    return false;
  }
  
  displayNumber++;
  for(uint8_t chunk = 1; chunk < m_displays[displayIndex_]->getNumberOfChunks() - 1 ; chunk++ )
  {
    offset += dataSize;
    if(!getDeviceHandle()->write(
        Transfer({ displayNumber, 0x01, 0xF6 },
          m_displays[displayIndex_]->getPtr( offset ),
          dataSize
        ),
        kMASMK1_endpointDisplay
    ))
    {
      return false;
    }
  }
  
  offset += dataSize;
  
  if(!getDeviceHandle()->write(
    Transfer(
      { displayNumber, 0x01, 0x52 },
      m_displays[displayIndex_]->getPtr( offset ),
      338
    ),
    kMASMK1_endpointDisplay
  ))
  {
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::sendLeds()
{
  if( m_isDirtyLedGroup0 )
  {
    if(!getDeviceHandle()->write( Transfer( { 0x0C, 0x00}, &m_leds[0],  31 ), kMASMK1_endpointOut ))
    {
      return false;
    }
    m_isDirtyLedGroup0 = false;
  }
  
  if( m_isDirtyLedGroup1 )
  {
    if(!getDeviceHandle()->write( Transfer( { 0x0C, 0x1E}, &m_leds[31], 31 ), kMASMK1_endpointOut ))
    {
      return false;
    }
    m_isDirtyLedGroup1 = false;
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::read()
{
  Transfer input;
  

  if(getDeviceHandle()->read( input, kMASMK1_endpointInputPads ) )
  {
    processPads(input);
  }

  if( getDeviceHandle()->read( input, kMASMK1_endpointInputButtonsAndDials ) )
  {

    if(input[0] == 0x02)
    {
      getDeviceHandle()->write(Transfer({ 0x02, 0xFF, 0x02, 0x05 }), kMASMK1_endpointOut);
      processEncoders(input);
    }
    else if (input[0] == 0x04)
    {
      getDeviceHandle()->write(Transfer({ 0x04, 0xFF, 0x02, 0x05 }), kMASMK1_endpointOut);
      processButtons(input);
    }
    else 
    {
      getDeviceHandle()->write(Transfer({ 0xB0, 0xFF, 0x02, 0x05 }), kMASMK1_endpointOut);
    }
  }
  else
  {
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::processPads(const Transfer& input_)
{
  //\todo process pad data
  for (int i = 1; i < kMASMK1_padDataSize-1; i += 2)
  {
    uint16_t l = input_[i];
    uint16_t h = input_[i + 1];
    uint8_t pad = (h & 0xF0) >> 4;
    m_padsRawData[pad].write(((h & 0x0F) << 8) | l);
    m_padsAvgData[pad] = (((h & 0x0F) << 8) | l);

    Device::Pad btn(Device::Pad::Unknown);

#define M_PAD_CASE(value, pad) \
  case value:                  \
    btn = Device::Pad::pad; \
    break

    switch (pad)
    {
      M_PAD_CASE(0, Pad13);
      M_PAD_CASE(1, Pad14);
      M_PAD_CASE(2, Pad15);
      M_PAD_CASE(3, Pad16);
      M_PAD_CASE(4, Pad9);
      M_PAD_CASE(5, Pad10);
      M_PAD_CASE(6, Pad11);
      M_PAD_CASE(7, Pad12);
      M_PAD_CASE(8, Pad5);
      M_PAD_CASE(9, Pad6);
      M_PAD_CASE(10, Pad7);
      M_PAD_CASE(11, Pad8);
      M_PAD_CASE(12, Pad1);
      M_PAD_CASE(13, Pad2);
      M_PAD_CASE(14, Pad3);
      M_PAD_CASE(15, Pad4);
    }

#undef M_PAD_CASE

    if (m_padsAvgData[pad] > 1000)
    {
      padChanged(btn, m_padsAvgData[pad], m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);
  if ((input_.getData()[6] & 0x40) == 0)
  {
    return;
  }
  for (int i = 0; i < kMASMK1_buttonsDataSize; i++)
  {
    for (int k = 0; k < 8; k++)
    {
      uint8_t btn = (i * 8) + k;
      Button currentButton(static_cast<Button>(btn));
      if (currentButton == Button::Shift || currentButton > Button::Play)
      {
        continue;
      }
      buttonPressed = isButtonPressed(input_, currentButton);
      if (buttonPressed != m_buttonStates[btn])
      {
        m_buttonStates[btn] = buttonPressed;
        changedButton = getDeviceButton(currentButton);
        if (changedButton != Device::Button::Unknown)
        {
          //    std::copy(&input_[1],&input_[kMikroMK2_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
        }
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::processEncoders(const Transfer& input_)
{/*
  if ((input_.getData()[6] & 0x80) == 0)
  {
    return;
  }*/
  for (uint8_t i = 0; i < kMASMK1_nEncoders; i++)
  {
    Encoder currentEnc(static_cast<Encoder>(i));
    uint16_t currentEncValue = (input_.getData()[1 + (2 * i)] & 0xf0);
    if (m_encoderValues[i] == currentEncValue)
    {
      continue;
    }
    bool valueIncreased
      = ((m_encoderValues[i] < currentEncValue) || ((currentEncValue == 0xffff) && (currentEncValue == 0x00)))
      && (!((m_encoderValues[i] == 0x0) && (m_encoderValues[i] == 0xffff)));
    if (m_encodersInitialized)
    {
      encoderChanged(getDeviceEncoder(currentEnc), valueIncreased, m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
    }
    m_encoderValues[i] = currentEncValue;
  }
  m_encodersInitialized = true;
}

//----------------------------------------------------------------------------------------------------------------------

DeviceMaschineMK1::Led DeviceMaschineMK1::getLed( Device::Button btn_ ) const noexcept
{
#define M_LED_CASE(idLed) case Device::Button::idLed: return Led::idLed

  switch ( btn_ )
  {
    M_LED_CASE(Pad1);
    M_LED_CASE(Pad2);
    M_LED_CASE(Pad3);
    M_LED_CASE(Pad4);
    M_LED_CASE(Pad5);
    M_LED_CASE(Pad6);
    M_LED_CASE(Pad7);
    M_LED_CASE(Pad8);
    M_LED_CASE(Pad9);
    M_LED_CASE(Pad10);
    M_LED_CASE(Pad11);
    M_LED_CASE(Pad12);
    M_LED_CASE(Pad13);
    M_LED_CASE(Pad14);
    M_LED_CASE(Pad15);
    M_LED_CASE(Pad16);  
    M_LED_CASE(Mute);
    M_LED_CASE(Solo);
    M_LED_CASE(Select);
    M_LED_CASE(Duplicate);
    M_LED_CASE(Navigate);
    M_LED_CASE(Keyboard);
    M_LED_CASE(Pattern);
    M_LED_CASE(Scene);
    M_LED_CASE(Shift);
    M_LED_CASE(Erase);
    M_LED_CASE(Grid);
    M_LED_CASE(TransportRight);
    M_LED_CASE(Rec);
    M_LED_CASE(Play);  
    M_LED_CASE(TransportLeft);
    M_LED_CASE(Loop);
    M_LED_CASE(GroupA);
    M_LED_CASE(GroupB);
    M_LED_CASE(GroupC);
    M_LED_CASE(GroupD);
    M_LED_CASE(GroupE);
    M_LED_CASE(GroupF);
    M_LED_CASE(GroupG);
    M_LED_CASE(GroupH);   
    M_LED_CASE(AutoWrite);
    M_LED_CASE(Snap);
    M_LED_CASE(BrowseRight);
    M_LED_CASE(BrowseLeft);
    M_LED_CASE(Sampling);
    M_LED_CASE(Browse);
    M_LED_CASE(Step);
    M_LED_CASE(Control);
    M_LED_CASE(DisplayButton1);
    M_LED_CASE(DisplayButton2);
    M_LED_CASE(DisplayButton3);
    M_LED_CASE(DisplayButton4);
    M_LED_CASE(DisplayButton5);
    M_LED_CASE(DisplayButton6);
    M_LED_CASE(DisplayButton7);
    M_LED_CASE(DisplayButton8);   
    M_LED_CASE(NoteRepeat);
    
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------

Device::Button DeviceMaschineMK1::getDeviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(Mute);
    M_BTN_CASE(Solo);
    M_BTN_CASE(Select);
    M_BTN_CASE(Duplicate);
    M_BTN_CASE(Navigate);
    M_BTN_CASE(Keyboard);
    M_BTN_CASE(Pattern);
    M_BTN_CASE(Scene);
    M_BTN_CASE(Rec);
    M_BTN_CASE(Erase);
    M_BTN_CASE(Shift);
    M_BTN_CASE(Grid);
    M_BTN_CASE(TransportRight);
    M_BTN_CASE(TransportLeft);
    M_BTN_CASE(Loop);
    M_BTN_CASE(GroupE);
    M_BTN_CASE(GroupF);
    M_BTN_CASE(GroupG);
    M_BTN_CASE(GroupH);
    M_BTN_CASE(GroupD);
    M_BTN_CASE(GroupC);
    M_BTN_CASE(GroupB);
    M_BTN_CASE(GroupA);
    M_BTN_CASE(Control);
    M_BTN_CASE(Browse);
    M_BTN_CASE(BrowseLeft);
    M_BTN_CASE(Snap);
    M_BTN_CASE(AutoWrite);
    M_BTN_CASE(BrowseRight);
    M_BTN_CASE(Sampling);
    M_BTN_CASE(Step);
    M_BTN_CASE(DisplayButton8);
    M_BTN_CASE(DisplayButton7);
    M_BTN_CASE(DisplayButton6);
    M_BTN_CASE(DisplayButton5);
    M_BTN_CASE(DisplayButton4);
    M_BTN_CASE(DisplayButton3);
    M_BTN_CASE(DisplayButton2);
    M_BTN_CASE(DisplayButton1);
    M_BTN_CASE(NoteRepeat);
    M_BTN_CASE(Play);

    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------

Device::Encoder DeviceMaschineMK1::getDeviceEncoder(Encoder btn_) const noexcept
{
#define M_ENCODER_CASE(idEncoder) \
  case Encoder::idEncoder:     \
    return Device::Encoder::idEncoder

  switch (btn_)
  {
    M_ENCODER_CASE(Volume);
    M_ENCODER_CASE(Tempo);
    M_ENCODER_CASE(Swing);
    M_ENCODER_CASE(Encoder1);
    M_ENCODER_CASE(Encoder2);
    M_ENCODER_CASE(Encoder3);
    M_ENCODER_CASE(Encoder4);
    M_ENCODER_CASE(Encoder5);
    M_ENCODER_CASE(Encoder6);
    M_ENCODER_CASE(Encoder7);
    M_ENCODER_CASE(Encoder8);

    default:
    {
      return Device::Encoder::Unknown;
    }
  }
#undef M_ENCODER_CASE
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
