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

#include "devices/DeviceKompleteKontrol.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions.h"

//----------------------------------------------------------------------------------------------------------------------

namespace
{
static const uint8_t kKK_epDisplay = 0x08;
static const uint8_t kKK_epOut     = 0x02;
static const uint8_t kKK_epInput   = 0x84;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

enum class DeviceKompleteKontrol::Led : uint8_t
{
  F1,
  F2,
  F3,
  Control,
  Nav,
  BrowseLeft,
  BrowseRight,
  Main,
  Group,
  GroupR = Group,
  GroupG,
  GroupB,
  Browse,
  Sampling,
  NoteRepeat,
  Restart,
  TransportLeft,
  TransportRight,
  Grid,
  Play,
  Rec,
  Erase,
  Shift,
  Scene,
  Pattern,
  PadMode,
  View,
  Duplicate,
  Select,
  Solo,
  Mute,
  Pad13,
  Pad13R = Pad13,
  Pad13G,
  Pad13B,
  Pad14,
  Pad14R = Pad14,
  Pad14G,
  Pad14B,
  Pad15,
  Pad15R = Pad15,
  Pad15G,
  Pad15B,
  Pad16,
  Pad16R = Pad16,
  Pad16G,
  Pad16B,
  Pad9,
  Pad9R = Pad9,
  Pad9G,
  Pad9B,
  Pad10,
  Pad10R = Pad10,
  Pad10G,
  Pad10B,
  Pad11,
  Pad11R = Pad11,
  Pad11G,
  Pad11B,
  Pad12,
  Pad12R = Pad12,
  Pad12G,
  Pad12H,
  Pad5,
  Pad5R = Pad5,
  Pad5G,
  Pad5B,
  Pad6,
  Pad6R = Pad6,
  Pad6G,
  Pad6B,
  Pad7,
  Pad7R = Pad7,
  Pad7G,
  Pad7B,
  Pad8,
  Pad8R = Pad8,
  Pad8G,
  Pad8B,
  Pad1,
  Pad1R = Pad1,
  Pad1G,
  Pad1B,
  Pad2,
  Pad2R = Pad2,
  Pad2G,
  Pad2B,
  Pad3,
  Pad3R = Pad3,
  Pad3G,
  Pad3B,
  Pad4,
  Pad4R = Pad4,
  Pad4G,
  Pad4B,
  Unknown,
};

//----------------------------------------------------------------------------------------------------------------------

enum class DeviceKompleteKontrol::Button : uint8_t
{
  Shift,
  Erase,
  Rec,
  Play,
  Grid,
  TransportRight,
  TransportLeft,
  Restart,

  MainEncoder = 11,
  NoteRepeat,
  Sampling,
  Browse,
  Group,

  Main,
  BrowseRight,
  BrowseLeft,
  Nav,
  Control,
  F3,
  F2,
  F1,

  Mute,
  Solo,
  Select,
  Duplicate,
  View,
  PadMode,
  Pattern,
  Scene,

  Pad13,
  Pad14,
  Pad15,
  Pad16,
  Pad9,
  Pad10,
  Pad11,
  Pad12,
  Pad5,
  Pad6,
  Pad7,
  Pad8,
  Pad1,
  Pad2,
  Pad3,
  Pad4,
  None,
};

//----------------------------------------------------------------------------------------------------------------------

DeviceKompleteKontrol::DeviceKompleteKontrol(tPtr<DeviceHandle> pDeviceHandle_, uint8_t numKeys_)
  : Device(std::move(pDeviceHandle_))
  , m_numKeys(numKeys_)
  , m_isDirtyLeds(false)
{
 //m_buttons.resize(kKK_buttonsDataSize);
 //m_leds.resize(kKK_ledsDataSize);
}

//----------------------------------------------------------------------------------------------------------------------

DeviceKompleteKontrol::~DeviceKompleteKontrol()
{

}

//----------------------------------------------------------------------------------------------------------------------

void DeviceKompleteKontrol::setLed(Device::Button btn_, const util::LedColor& color_)
{
  setLedImpl(getLed(btn_), color_);
}

//----------------------------------------------------------------------------------------------------------------------
/*
void DeviceKompleteKontrol::setLed(Device::Pad pad_, const util::LedColor& color_)
{
  setLedImpl(getLed(pad_), color_);
}
*/
//----------------------------------------------------------------------------------------------------------------------

void DeviceKompleteKontrol::sendMidiMsg(tRawData midiMsg_)
{
 //\todo Use KompleteKontrol hardware midi port
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay* DeviceKompleteKontrol::getDisplay(uint8_t displayIndex_)
{
  return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::tick()
{
  static int state = 0;
  bool success = false;

  if (state == 0)
  {
    success = sendLeds();
  }
  else if (state == 1)
  {
    success = read();
  }

  if (++state >= 2)
  {
    state = 0;
  }

  return success;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::sendLeds()
{
  if (m_isDirtyLeds)
  {
    if(!getDeviceHandle()->write(Transfer({0x80}, &m_leds[0], 78), kKK_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::read()
{
  Transfer input;
  for (uint8_t n = 0; n < 32; n++)
  {
    if (!getDeviceHandle()->read(input, kKK_epInput))
    {
      return false;
    }
    else if (input && input[0] == 0x01)
    {
      processButtons(input);
      break;
    }

/*
        std::cout << std::setfill('0') << std::internal;

        for( int i = 0; i < input.getSize(); i++ )
        {
          std::cout << std::hex << std::setw(2) << (int)input[i] <<  std::dec << " " ;
        }

        std::cout << std::endl << std::endl;*/
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceKompleteKontrol::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kKK_buttonsDataSize - 1; i++) // Skip the last byte (encoder value)
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
      //    std::copy(&input_[1],&input_[kKK_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
          }
        }
      }

    // Now process the encoder data
    uint8_t currentEncoderValue = input_.getData()[kKK_buttonsDataSize];
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

void DeviceKompleteKontrol::setLedImpl(Led led_, const util::LedColor& color_)
{
  uint8_t ledIndex = static_cast<uint8_t>(led_);

  if (isRGBLed(led_))
  {
    uint8_t currentR = m_leds[ledIndex];
    uint8_t currentG = m_leds[ledIndex + 1];
    uint8_t currentB = m_leds[ledIndex + 2];

    m_leds[ledIndex] = color_.getRed();
    m_leds[ledIndex + 1] = color_.getGreen();
    m_leds[ledIndex + 2] = color_.getBlue();

    m_isDirtyLeds = m_isDirtyLeds ||
      (currentR != color_.getRed() || currentG != color_.getGreen() || currentB != color_.getBlue());
  }
  else if (Led::Unknown != led_)
  {
    uint8_t currentVal = m_leds[ledIndex];
    uint8_t newVal = color_.getMono();

    m_leds[ledIndex] = newVal;
    m_isDirtyLeds = m_isDirtyLeds || (currentVal != newVal);
  }
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::isRGBLed(Led led_)
{
  if (Led::Group == led_ || Led::Pad1 == led_ || Led::Pad2 == led_ || Led::Pad3 == led_ || Led::Pad4 == led_
      || Led::Pad5 == led_ || Led::Pad6 == led_ || Led::Pad7 == led_ || Led::Pad8 == led_ || Led::Pad9 == led_
      || Led::Pad10 == led_ || Led::Pad11 == led_ || Led::Pad12 == led_ || Led::Pad13 == led_ || Led::Pad14 == led_
      || Led::Pad15 == led_ || Led::Pad16 == led_)
  {
    return true;
  }

  return false;
}

//----------------------------------------------------------------------------------------------------------------------

DeviceKompleteKontrol::Led DeviceKompleteKontrol::getLed(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(F1);
    M_LED_CASE(F2);
    M_LED_CASE(F3);
    M_LED_CASE(Control);
    M_LED_CASE(Nav);
    M_LED_CASE(BrowseLeft);
    M_LED_CASE(BrowseRight);
    M_LED_CASE(Main);
    M_LED_CASE(Group);
    M_LED_CASE(Browse);
    M_LED_CASE(Sampling);
    M_LED_CASE(NoteRepeat);
    M_LED_CASE(Restart);
    M_LED_CASE(TransportLeft);
    M_LED_CASE(TransportRight);
    M_LED_CASE(Grid);
    M_LED_CASE(Play);
    M_LED_CASE(Rec);
    M_LED_CASE(Erase);
    M_LED_CASE(Shift);
    M_LED_CASE(Scene);
    M_LED_CASE(Pattern);
    M_LED_CASE(PadMode);
    M_LED_CASE(View);
    M_LED_CASE(Duplicate);
    M_LED_CASE(Select);
    M_LED_CASE(Solo);
    M_LED_CASE(Mute);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------
/*
DeviceKompleteKontrol::Led DeviceKompleteKontrol::getLed(Device::Pad pad_) const noexcept
{
#define M_PAD_CASE(idPad)     \
  case Device::Pad::idPad: \
    return Led::idPad

  switch (pad_)
  {
    M_PAD_CASE(Pad13);
    M_PAD_CASE(Pad14);
    M_PAD_CASE(Pad15);
    M_PAD_CASE(Pad16);
    M_PAD_CASE(Pad9);
    M_PAD_CASE(Pad10);
    M_PAD_CASE(Pad11);
    M_PAD_CASE(Pad12);
    M_PAD_CASE(Pad5);
    M_PAD_CASE(Pad6);
    M_PAD_CASE(Pad7);
    M_PAD_CASE(Pad8);
    M_PAD_CASE(Pad1);
    M_PAD_CASE(Pad2);
    M_PAD_CASE(Pad3);
    M_PAD_CASE(Pad4);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_PAD_CASE
}
*/
//----------------------------------------------------------------------------------------------------------------------

Device::Button DeviceKompleteKontrol::getDeviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(F1);
    M_BTN_CASE(F2);
    M_BTN_CASE(F3);
    M_BTN_CASE(Control);
    M_BTN_CASE(Nav);
    M_BTN_CASE(BrowseLeft);
    M_BTN_CASE(BrowseRight);
    M_BTN_CASE(Main);
    M_BTN_CASE(Group);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Sampling);
    M_BTN_CASE(NoteRepeat);
    M_BTN_CASE(Restart);
    M_BTN_CASE(TransportLeft);
    M_BTN_CASE(TransportRight);
    M_BTN_CASE(Grid);
    M_BTN_CASE(Play);
    M_BTN_CASE(Rec);
    M_BTN_CASE(Erase);
    M_BTN_CASE(Shift);
    M_BTN_CASE(Scene);
    M_BTN_CASE(Pattern);
    M_BTN_CASE(PadMode);
    M_BTN_CASE(View);
    M_BTN_CASE(Duplicate);
    M_BTN_CASE(Select);
    M_BTN_CASE(Solo);
    M_BTN_CASE(Mute);
    M_BTN_CASE(MainEncoder);
    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceKompleteKontrol::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
