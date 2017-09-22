/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineMikroMK1.h"
#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"

#include <thread>

#include "cabl/gfx/TextDisplay.h"
#include "gfx/displays/NullCanvas.h"

//!\todo delete debug includes
#include <iomanip>
#include <iostream>

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kMikroMK1_epDisplay = 0x08;
const uint8_t kMikroMK1_epOut = 0x01;
const uint8_t kMikroMK1_epInput = 0x84;
const unsigned kMikroMK1_padThreshold = 200;
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

// clang-format off
enum class MaschineMikroMK1::Led : uint8_t
{
  Restart,
  NoteRepeat,
  Sampling,
  Browse,
  BrowseLeft,
  BrowseRight,
  Grid,
  Erase,
  Group,
  Rec,
  Play,
  Shift,
  View,
  PadMode,
  Pattern,
  Scene,
  Duplicate,
  Select,
  Solo,
  Mute,

  Pad4,
  Pad3,
  Pad2,
  Pad1,
  Pad8,
  Pad7,
  Pad6,
  Pad5,
  Pad16B,
  Pad12B,
  Pad15B,
  Pad11B,
  Pad14B,
  Pad13B,
  Pad9B,
  Pad10B,
  Main,
  F3,
  F2,
  F1,
  Nav,
  TransportLeft,
  TransportRight,
  Enter,
  Pad12,
  Pad11,
  Pad10,
  Pad9,
  Pad16,
  Pad15,
  Pad14,
  Pad13,

  Unknown,
};
// clang-format on

//--------------------------------------------------------------------------------------------------


enum class MaschineMikroMK1::Button : uint8_t
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

  Enter,
  BrowseRight,
  BrowseLeft,
  Nav,
  Main,
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

  None,
};

//--------------------------------------------------------------------------------------------------

MaschineMikroMK1::MaschineMikroMK1() : m_padsStatus(0), m_isDirtyLeds(false)
{
}

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

Canvas* MaschineMikroMK1::graphicDisplay(size_t displayIndex_)
{
  static NullCanvas s_dummyDisplay;
  if (displayIndex_ > 0)
  {
    return &s_dummyDisplay;
  }

  return &m_display;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::tick()
{
  static int state = 0;
  bool success = false;

  //!\todo enable once display dirty flag is properly set
  if (state == 0 && m_display.dirty())
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

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::init()
{
  // Display
  initDisplay();
  m_display.white();

  // Leds
  std::fill(std::begin(m_leds), std::end(m_leds), 0);
  m_isDirtyLeds = true;
}

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::initDisplay() const
{
  //!\todo set backlight
  return;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::sendFrame()
{
  uint8_t yOffset = 0;
  for (int chunk = 0; chunk < 4; chunk++, yOffset += 2)
  {
    const uint8_t* ptr = m_display.buffer() + (chunk * 256);
    if (!writeToDeviceHandle(
          Transfer({0xE0, 0x00, 0x00, yOffset, 0x00, 0x80, 0x00, 0x02, 0x00}, ptr, 256),
          kMikroMK1_epDisplay))
    {
      return false;
    }
  }
  m_display.resetDirtyFlags();
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::sendLeds()
{
  //  if (m_isDirtyLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x80}, &m_leds[0], 78), kMikroMK1_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::read()
{
  Transfer input;
  for (uint8_t n = 0; n < 32; n++)
  {
    if (!readFromDeviceHandle(input, kMikroMK1_epInput))
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

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  m_buttonStates[static_cast<unsigned>(Button::Shift)] = shiftPressed;

  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kMikroMK1_buttonsDataSize - 1; i++) // Skip the last byte (encoder value)
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
        changedButton = deviceButton(currentButton);
        if (changedButton != Device::Button::Unknown)
        {
          //    std::copy(&input_[1],&input_[kMikroMK1_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
        }
      }
    }
  }

  // Now process the encoder data
  uint8_t currentEncoderValue = input_.data()[kMikroMK1_buttonsDataSize];
  if (m_encoderValue != currentEncoderValue)
  {
    bool valueIncreased = ((m_encoderValue < currentEncoderValue)
                            || ((m_encoderValue == 0x0f) && (currentEncoderValue == 0x00)))
                          && (!((m_encoderValue == 0x0) && (currentEncoderValue == 0x0f)));
    encoderChanged(0, valueIncreased, shiftPressed);
    m_encoderValue = currentEncoderValue;
  }
}

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::processPads(const Transfer& input_)
{
  //!\todo process pad data
  for (int i = 1; i <= kMikroMK1_padDataSize; i += 2)
  {
    unsigned l = input_[i];
    unsigned h = input_[i + 1];
    uint8_t pad = (h & 0xF0) >> 4;
    m_padsData[pad] = (((h & 0x0F) << 8) | l);

    if (m_padsData[pad] > kMikroMK1_padThreshold)
    {
      m_padsStatus[pad] = true;
      keyChanged(
        pad, m_padsData[pad] / 1024.0, m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
    }
    else
    {
      if (m_padsStatus[pad])
      {
        m_padsStatus[pad] = false;
        keyChanged(pad, 0.0, m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void MaschineMikroMK1::setLedImpl(Led led_, const Color& color_)
{
  uint8_t ledIndex = static_cast<uint8_t>(led_);

  if (Led::Unknown == led_)
  {
    return;
  }

  if (isRGBLed(led_))
  {
    uint8_t currentR = m_leds[ledIndex];
    uint8_t currentG = m_leds[ledIndex + 1];
    uint8_t currentB = m_leds[ledIndex + 2];

    m_leds[ledIndex] = color_.red();
    m_leds[ledIndex + 1] = color_.green();
    m_leds[ledIndex + 2] = color_.blue();

    m_isDirtyLeds = m_isDirtyLeds || (currentR != color_.red() || currentG != color_.green()
                                       || currentB != color_.blue());
  }
  else
  {
    uint8_t currentVal = m_leds[ledIndex];
    uint8_t newVal = color_.mono();

    m_leds[ledIndex] = newVal;
    m_isDirtyLeds = m_isDirtyLeds || (currentVal != newVal);
  }
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::isRGBLed(Led led_) const noexcept
{
  if (Led::Group == led_ || Led::Pad1 == led_ || Led::Pad2 == led_ || Led::Pad3 == led_
      || Led::Pad4 == led_
      || Led::Pad5 == led_
      || Led::Pad6 == led_
      || Led::Pad7 == led_
      || Led::Pad8 == led_
      || Led::Pad9 == led_
      || Led::Pad10 == led_
      || Led::Pad11 == led_
      || Led::Pad12 == led_
      || Led::Pad13 == led_
      || Led::Pad14 == led_
      || Led::Pad15 == led_
      || Led::Pad16 == led_)
  {
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

MaschineMikroMK1::Led MaschineMikroMK1::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(F1);
    M_LED_CASE(F2);
    M_LED_CASE(F3);
    M_LED_CASE(Main);
    M_LED_CASE(Nav);
    M_LED_CASE(BrowseLeft);
    M_LED_CASE(BrowseRight);
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

//--------------------------------------------------------------------------------------------------

MaschineMikroMK1::Led MaschineMikroMK1::led(unsigned index_) const noexcept
{
#define M_LED_CASE(val, idLed) \
  case val:                    \
    return Led::idLed
  switch (index_)
  {
    M_LED_CASE(0, Pad13);
    M_LED_CASE(1, Pad14);
    M_LED_CASE(2, Pad15);
    M_LED_CASE(3, Pad16);
    M_LED_CASE(4, Pad9);
    M_LED_CASE(5, Pad10);
    M_LED_CASE(6, Pad11);
    M_LED_CASE(7, Pad12);
    M_LED_CASE(8, Pad5);
    M_LED_CASE(9, Pad6);
    M_LED_CASE(10, Pad7);
    M_LED_CASE(11, Pad8);
    M_LED_CASE(12, Pad1);
    M_LED_CASE(13, Pad2);
    M_LED_CASE(14, Pad3);
    M_LED_CASE(15, Pad4);
    default:
    {
      return Led::Unknown;
    }
  }
#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

Device::Button MaschineMikroMK1::deviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(F1);
    M_BTN_CASE(F2);
    M_BTN_CASE(F3);
    M_BTN_CASE(Main);
    M_BTN_CASE(Nav);
    M_BTN_CASE(BrowseLeft);
    M_BTN_CASE(BrowseRight);
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

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool MaschineMikroMK1::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
