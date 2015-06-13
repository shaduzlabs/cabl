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
  F2,
  F1,
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

DeviceMaschineMK1::DeviceMaschineMK1()
  : Device( Driver::tDriver::LIBUSB )
  , m_pendingAcks( 0 )
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
  
bool DeviceMaschineMK1::connect()
{
  if( !getDeviceHandle().connect( kMASMK1_vendorId, kMASMK1_productId ) )
  {
    return false;
  }
  
  init();

  return true;
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
 //     sendFrame(i);
    }
  }
  if( m_isDirtyLedGroup0 || m_isDirtyLedGroup1 )
  {
    if(!sendLeds())
    {
      return false;
    }
  }
//  getDeviceHandle().write( Transfer({ 0x0C, 0xFF, 0x02, 0x05 ), 1 );
  if(!read())
  {
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK1::init()
{
  // Displays
  for( int i=0; i< kMASMK1_nDisplays; i++ )
  {
    initDisplay(i);
    m_displays[i].get()->white();
  }
  
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
  
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x30                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x04, 0xCA, 0x04, 0x0F, 0x00 }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x02, 0xBB, 0x00             }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0xD1                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x94                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x81, 0x1E, 0x02       }), kMASMK1_endpointDisplay );
  
  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x02, 0x20, 0x08             }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x02, 0x20, 0x0B             }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0xA6                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x31                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x04, 0x32, 0x00, 0x00, 0x05 }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x34                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x30                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x04, 0xBC, 0x00, 0x01, 0x02 }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x75, 0x00, 0x3F       }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x15, 0x00, 0x54       }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x5C                   }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0x25                   }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0xAF                   }), kMASMK1_endpointDisplay );

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x04, 0xBC, 0x02, 0x01, 0x01 }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x01, 0xA6                   }), kMASMK1_endpointDisplay );
  
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x81, 0x09, 0x04       }), kMASMK1_endpointDisplay );
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
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x75, 0x00, 0x3F }), kMASMK1_endpointDisplay );
  getDeviceHandle().write( Transfer({ displayNumber, 0x00, 0x03, 0x15, 0x00, 0x54 }), kMASMK1_endpointDisplay );
  
  uint16_t offset = 0;
  const uint16_t dataSize = 502;
  
  if(!getDeviceHandle().write(
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
    if(!getDeviceHandle().write(
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
  
  if(!getDeviceHandle().write(
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
    if(!getDeviceHandle().write( Transfer( { 0x0C, 0x00}, &m_leds[0],  31 ), kMASMK1_endpointOut ))
    {
      return false;
    }
    m_pendingAcks++;
    m_isDirtyLedGroup0 = false;
  }
  
  if( m_isDirtyLedGroup1 )
  {
    if(!getDeviceHandle().write( Transfer( { 0x0C, 0x1E}, &m_leds[31], 31 ), kMASMK1_endpointOut ))
    {
      return false;
    }
    m_pendingAcks++;
    m_isDirtyLedGroup1 = false;
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK1::read()
{
  Transfer input;
  if( !getDeviceHandle().read( input, kMASMK1_endpointInputPads ) )
  {
    return false;
  /*
//    std::cout << "Packet #IN (" << input.getSize() << "bytes):" << std::endl;
    
    std::cout << std::setfill('0') << std::internal;
   
    for( int i = 0; i < input.getSize(); i++ )
    {
      std::cout << std::hex << std::setw(2) << (int)input[i] <<  std::dec << " " ;
    }

    std::cout << std::endl << std::endl;*/
  }

 // getDeviceHandle().write( Transfer({ 0x02, 0xFF, 0x02, 0x05 ), kMASMK1_endpointOut );

  // Request dials data
  if( getDeviceHandle().read( input, kMASMK1_endpointInputButtonsAndDials ) )
  {
    if(input[0] == 0x02)
    {
      std::cout << "Dial Packet #IN (" << input.size() << "bytes):" << std::endl;
    
      std::cout << std::setfill('0') << std::internal;
      for( size_t i = 0; i < input.size(); i++ )
      {
        std::cout << std::hex << std::setw(2) << (int)input[i] <<  std::dec << " " ;
      }

      std::cout << std::endl << std::endl;
    }
  }
  else
  {
    return false;
  }

  // Request button data
  getDeviceHandle().write( Transfer({ 0x04, 0xFF, 0x02, 0x05 }), kMASMK1_endpointOut );
  if( getDeviceHandle().read( input, kMASMK1_endpointInputButtonsAndDials ) )
  {    
    if(input[0] != 0x02)
    {
      std::cout << "Button Packet #IN (" << input.size() << "bytes):" << std::endl;
    
      std::cout << std::setfill('0') << std::internal;
      for(size_t i = 0; i < input.size(); i++ )
      {
        std::cout << std::hex << std::setw(2) << (int)input[i] <<  std::dec << " " ;
      }

      std::cout << std::endl << std::endl;
    }
  }
  else
  {
    return false;
  }

  return true;

  /*
  
  byte 0 = 0x04
  
  byte 1
  -----------
  MUTE        = 0x01
  SOLO        = 0x02
  SELECT      = 0x04
  DUPLICATE   = 0x08
  NAVIGATE    = 0x10
  KEYBOARD    = 0x20
  PATTERN     = 0x40
  SCENE       = 0x80
  
  byte 2
  -----------
  N/A
  REC
  ERASE
  SHIFT
  GRID
  NAV_RIGHT
  NAV_LEFT
  LOOP
  
  byte 3
  -----------
  GROUP E
  GROUP F
  GROUP G
  GROUP H
  GROUP D
  GROUP C
  GROUP B
  GROUP A
  

  byte 4
  -----------
  CONTROL
  BROWSE
  BRWS LEFT
  F1
  F2
  BRWS RIGHT
  SAMPLING
  STEP
  
  byte 5
  -----------
  B8  
  B7
  B6
  B5
  B4
  B3
  B2
  B1
  
  
  byte 6
  -----------
  NOTE REPEAT = 0x01
  PLAY        = 0x02
  N/A
  N/A
  N/A
  N/A
  ?? Button  changed flag?
  ?? Encoder changed flag?
  
  byte 7
  -----------
  N/A
  N/A
  N/A
  N/A
  N/A
  N/A
  N/A
  N/A



Dials

  Byte 0 = 0x02
  
  Byte 1,2   = Dial 8
  Byte 3,4   = Dial 4
  Byte 5,6   = Swing
  Byte 7,8   = Dial 7
  Byte 9,10  = Dial 3
  Byte 11,12 = Tempo
  Byte 13,14 = Dial 6
  Byte 15,16 = Dial 2
  Byte 17,18 = Volume
  Byte 19,20 = Dial 5
  Byte 21,22 = Dial 1
  byte 23,24,25,26,27,28,29,30,31,32 = N/A

  */
    
  /*
  if( m_pendingAcks > 1)
  {
    Transfer input;
    getDeviceHandle().read( input, 0x81 );
    m_pendingAcks--;
  }
*/
  
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
    M_LED_CASE(F2);
    M_LED_CASE(F1);
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

} // kio
} // sl
