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

#include "devices/DeviceMaschineMK2.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "Macros.h"
#include "util/Functions_SL.h"

#include <thread>

#include "gfx/displays/GDisplayMaschineMK2.h"

//\todo delete debug includes
#include <iostream>
#include <iomanip>

//----------------------------------------------------------------------------------------------------------------------

namespace
{
static const uint8_t kMASMK2_endpointDisplay = 0x08;
static const uint8_t kMASMK2_endpointLeds = 0x01;
static const uint8_t kMASMK2_endpointInput = 0x84;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------
enum class DeviceMaschineMK2::Led : uint8_t{
  Control,
  Step,
  Browse,
  Sampling,
  BrowseLeft,
  BrowseRight,
  All,
  AutoWrite,

  DisplayButton1,
  DisplayButton2,
  DisplayButton3,
  DisplayButton4,
  DisplayButton5,
  DisplayButton6,
  DisplayButton7,
  DisplayButton8,

  Scene,
  Pattern,
  PadMode,
  Navigate,
  Duplicate,
  Select,
  Solo,
  Mute,

  Volume,
  Swing,
  Tempo,
  MasterLeft,
  MasterRight,
  Enter,
  NoteRepeat,

  Restart = 49,
  StepLeft,
  StepRight,
  Grid,
  Play,
  Rec,
  Erase,
  Shift,
  
  
  Unknown,
};

//----------------------------------------------------------------------------------------------------------------------

enum class DeviceMaschineMK2::Button : uint8_t{
  P1,
  P2,
  P3,
  P4,
  P5,
  P6,
  P7,
  P8,

  Control,
  Step,
  Browse,
  Sampling,
  BrowseLeft,
  BrowseRight,
  All,
  AutoWrite,

  Volume,
  Swing,
  Tempo,
  MasterLeft,
  MasterRight,
  Enter,
  NoteRepeat,
  NotUsed,

  GroupA,
  GroupB,
  GroupC,
  GroupD,
  GroupE,
  GroupF,
  GroupG,
  GroupH,

  Restart,
  StepLeft,
  StepRight,
  Grid,
  Play,
  Rec,
  Erase,
  Shift,

  Scene,
  Pattern,
  PadMode,
  Navigate,
  Duplicate,
  Select,
  Solo,
  Mute,
};

//----------------------------------------------------------------------------------------------------------------------

DeviceMaschineMK2::DeviceMaschineMK2(tPtr<DeviceHandle> pDeviceHandle_)
  : Device(std::move(pDeviceHandle_))
  , m_isDirtyLeds(false)
{
  m_buttons.resize(kMASMK2_buttonsDataSize);
  m_leds.resize(kMASMK2_ledsDataSize);
  m_displays[0].reset( new GDisplayMaschineMK2 );
  m_displays[1].reset( new GDisplayMaschineMK2 );
}

//----------------------------------------------------------------------------------------------------------------------

DeviceMaschineMK2::~DeviceMaschineMK2()
{

}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::setLed(Device::Button btn_, uint8_t val_)
{
  Led led = getLed(btn_);

  if (isRGBLed(led))
  {
    uint8_t currentR = m_leds[static_cast<uint16_t>(led)];
    uint8_t currentG = m_leds[static_cast<uint16_t>(led) + 1];
    uint8_t currentB = m_leds[static_cast<uint16_t>(led) + 2];

    m_leds[static_cast<uint16_t>(led)] = val_;
    m_leds[static_cast<uint16_t>(led) + 1] = val_;
    m_leds[static_cast<uint16_t>(led) + 2] = val_;

    m_isDirtyLeds = (currentR != val_ || currentG != val_ || currentB != val_);
  }
  else if (Led::Unknown != led)
  {
    uint8_t currentVal = m_leds[static_cast<uint16_t>(led)];
    uint8_t newVal = val_;

    m_leds[static_cast<uint16_t>(led)] = newVal;
    m_isDirtyLeds = (currentVal != newVal);
  }
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::setLed(Device::Button btn_, uint8_t r_, uint8_t g_, uint8_t b_)
{
  Led led = getLed(btn_);

  if (isRGBLed(led))
  {
    uint8_t currentR = m_leds[static_cast<uint16_t>(led)];
    uint8_t currentG = m_leds[static_cast<uint16_t>(led) + 1];
    uint8_t currentB = m_leds[static_cast<uint16_t>(led) + 2];

    m_leds[static_cast<uint16_t>(led)] = r_;
    m_leds[static_cast<uint16_t>(led) + 1] = g_;
    m_leds[static_cast<uint16_t>(led) + 2] = b_;

    m_isDirtyLeds = (currentR != r_ || currentG != g_ || currentB != b_);
  }
  else if (Led::Unknown != led)
  {
    uint8_t currentVal = m_leds[static_cast<uint16_t>(led)];

    // Use "Maximum decomposition" -> take the channel with the highest value
    uint8_t newVal = util::max<uint8_t>(r_, g_, b_);

    m_leds[static_cast<uint16_t>(led)] = newVal;
    m_isDirtyLeds = (currentVal != newVal);
  }
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay* DeviceMaschineMK2::getDisplay(uint8_t displayIndex_)
{
  if (displayIndex_ > 1)
  {
    return nullptr;
  }

  return m_displays[displayIndex_].get();
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::tick()
{
  static int state = 0;
  bool success = false;

  //\todo enable once display dirty flag is properly set
  if (state == 0) //&& m_display->isDirty())
  {
    success = sendFrame();
  }

  else if (state == 1)
  {
    success = sendLeds();
  }
  else if (state == 2)
  {
    success = read();
  }

  if (++state >= 3)
  {
    state = 0;
  }

  return success;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::init()
{
  // Display
  initDisplay();
  m_displays[0].get()->white();
  m_displays[1].get()->white();

  // Leds
  m_isDirtyLeds = true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::initDisplay() const
{
  //\todo set backlight
  return;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::sendFrame()
{
  uint8_t yOffset = 0;
  for(uint8_t i=0; i<kMASMK2_nDisplays;i++)
  {
    for(uint8_t chunk = 0; chunk < 4; chunk++, yOffset += 2)
    {
      uint8_t firstByte = 0xE0|i;
      const uint8_t* ptr = m_displays[i]->getPtr(chunk * 256);
      if(!getDeviceHandle()->write(Transfer({firstByte, 0x00, 0x00, yOffset, 0x00, 0x80, 0x00, 0x02, 0x00}, ptr, 256),
                     kMASMK2_endpointDisplay))
      {
        return false;
      }
    }
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::sendLeds()
{
//  if (m_isDirtyLeds)
  {
    if(!getDeviceHandle()->write(Transfer({0x80}, &m_leds[0], 78), kMASMK2_endpointLeds))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::read()
{
  Transfer input;
  for (uint8_t n = 0; n < 32; n++)
  {
    if (!getDeviceHandle()->read(input, kMASMK2_endpointInput))
    {
      return false;
    }
    else if (input && input[0] == 0x01)
    {
      processButtons(input);
      break;
    }
    else if (input && input[0] == 0x20 && n % 8 == 0) // Too many pad messages, need to skip some...
    {
      processPads(input);
    }
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kMASMK2_buttonsDataSize - 1; i++) // Skip the last byte (encoder value)
  {
    for (int k = 0; k < 8; k++)
    {
      uint8_t btn = (i * 8) + k;
      Button currentButton(static_cast<Button>(btn));
      if (currentButton == Button::Shift)
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
      //    std::copy(&input_[1],&input_[kMASMK2_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
          }
        }
      }

    // Now process the encoder data
    uint8_t currentEncoderValue = input_.getData()[kMASMK2_buttonsDataSize];
    if (m_encoderValue != currentEncoderValue)
    {
      bool valueIncreased
        = ((m_encoderValue < currentEncoderValue) || ((m_encoderValue == 0x0f) && (currentEncoderValue == 0x00)))
          && (!((m_encoderValue == 0x0) && (currentEncoderValue == 0x0f)));
        encoderChanged(Device::Encoder::Main, valueIncreased, shiftPressed);
      m_encoderValue = currentEncoderValue;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceMaschineMK2::processPads(const Transfer& input_)
{
  //\todo process pad data
  for (int i = 1; i <= kMASMK2_padDataSize; i += 2)
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

bool DeviceMaschineMK2::isRGBLed(Led led_)
{
/*
  if (Led::GroupA == led_ || Led::GroupB == led_ || Led::GroupC == led_ || Led::GroupD == led_ || Led::GroupE == led_ || Led::GroupF == led_ || Led::GroupG == led_ || Led::GroupA == led_ || Led::Pad1 == led_ || Led::Pad2 == led_ || Led::Pad3 == led_ || Led::Pad4 == led_
      || Led::Pad5 == led_ || Led::Pad6 == led_ || Led::Pad7 == led_ || Led::Pad8 == led_ || Led::Pad9 == led_
      || Led::Pad10 == led_ || Led::Pad11 == led_ || Led::Pad12 == led_ || Led::Pad13 == led_ || Led::Pad14 == led_
      || Led::Pad15 == led_ || Led::Pad16 == led_)
  {
    return true;
  }
*/
  return false;
}

//----------------------------------------------------------------------------------------------------------------------

DeviceMaschineMK2::Led DeviceMaschineMK2::getLed(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Control);

    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------

Device::Button DeviceMaschineMK2::getDeviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(Control);


    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceMaschineMK2::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
