/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/TraktorF1MK2.h"
#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/gfx/TextDisplay.h"
#include "cabl/util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kF1MK2_epOut = 0x02;
const uint8_t kF1MK2_epInput = 0x84;
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/*
         A
      -------
     |       |
   F |       | B
     |   G   |
      -------
     |       |
   E |       | C
     |   D   |
      -------
*/

// clang-format off
enum class TraktorF1MK2::Led : unsigned
{
  Display2_segDP,
  Display2_SegG,
  Display2_SegC,
  Display2_SegB,
  Display2_SegA,
  Display2_SegF,
  Display2_SegE,
  Display2_SegD,

  Display1_segDP,
  Display1_SegG,
  Display1_SegC,
  Display1_SegB,
  Display1_SegA,
  Display1_SegF,
  Display1_SegE,
  Display1_SegD,

  Browse,
  Size,
  Type,
  Reverse,
  Shift,
  Capture,
  Quant,
  Sync,

  Pad13, Pad13_B = Pad13, Pad13_R, Pad13_G,
  Pad14, Pad14_B = Pad14, Pad14_R, Pad14_G,
  Pad15, Pad15_B = Pad15, Pad15_R, Pad15_G,
  Pad16, Pad16_B = Pad16, Pad16_R, Pad16_G,
  Pad9,  Pad9_B  = Pad9,  Pad9_R,  Pad9_G,
  Pad10, Pad10_B = Pad10, Pad10_R, Pad10_G,
  Pad11, Pad11_B = Pad11, Pad11_R, Pad11_G,
  Pad12, Pad12_B = Pad12, Pad12_R, Pad12_G,
  Pad5,  Pad5_B  = Pad5,  Pad5_R,  Pad5_G,
  Pad6,  Pad6_B  = Pad6,  Pad6_R,  Pad6_G,
  Pad7,  Pad7_B  = Pad7,  Pad7_R,  Pad7_G,
  Pad8,  Pad8_B  = Pad8,  Pad8_R,  Pad8_G,
  Pad1,  Pad1_B  = Pad1,  Pad1_R,  Pad1_G,
  Pad2,  Pad2_B  = Pad2,  Pad2_R,  Pad2_G,
  Pad3,  Pad3_B  = Pad3,  Pad3_R,  Pad3_G,
  Pad4,  Pad4_B  = Pad4,  Pad4_R,  Pad4_G,

  Stop4, Stop4_A = Stop4, Stop4_B,
  Stop3, Stop3_A = Stop3, Stop3_B,
  Stop2, Stop2_A = Stop2, Stop2_B,
  Stop1, Stop1_A = Stop1, Stop1_B,

  Unknown,
};
// clang-format on

//--------------------------------------------------------------------------------------------------

enum class TraktorF1MK2::Button : uint8_t
{
  Pad8,
  Pad7,
  Pad6,
  Pad5,
  Pad4,
  Pad3,
  Pad2,
  Pad1,

  Pad16,
  Pad15,
  Pad14,
  Pad13,
  Pad12,
  Pad11,
  Pad10,
  Pad9,

  MainEncoder = 18,
  Browse,
  Size,
  Type,
  Reverse,
  Shift,

  Capture = 25,
  Quant,
  Sync,
  Stop4,
  Stop3,
  Stop2,
  Stop1,

  None,
};

//--------------------------------------------------------------------------------------------------

TraktorF1MK2::TraktorF1MK2() : m_isDirtyLeds(true)
{
}

//--------------------------------------------------------------------------------------------------

void TraktorF1MK2::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void TraktorF1MK2::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

TextDisplay* TraktorF1MK2::textDisplay(size_t displayIndex_)
{
  static TextDisplayDummy s_dummyTextDisplay;
  if (displayIndex_ > 0)
  {
    return &s_dummyTextDisplay;
  }
  return &m_TextDisplay;
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::tick()
{
  static int state = 0;
  bool success = false;

  if (state == 0)
  {
    success = sendLedsAndDisplay();
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

//--------------------------------------------------------------------------------------------------

void TraktorF1MK2::init()
{
  std::fill(m_leds.begin(), m_leds.end(), 0);
  writeToDeviceHandle(Transfer({0xA0, 0x00, 0x00}), kF1MK2_epOut);
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::sendLedsAndDisplay()
{
  if (m_TextDisplay.dirty() || true)
  {
    const auto displayData = m_TextDisplay.displayData();
    for (size_t i = 0; i < m_TextDisplay.dataSize(); i++)
    {
      for (uint8_t j = 0; j < 8; j++)
      {
        size_t displayIndex = (m_TextDisplay.dataSize() - 1 - i);
        m_leds[(displayIndex * 8) + j] = (((1 << j) & displayData[i]) > 0) ? 0x7f : 0x00;
      }
    }
    m_isDirtyLeds = true;
  }
  if (m_isDirtyLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x80}, &m_leds[0], kF1MK2_nLeds), kF1MK2_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::read()
{
  Transfer input;

  if (!readFromDeviceHandle(input, kF1MK2_epInput))
  {
    return false;
  }
  else if (input && input[0] == 0x01)
  {
    processButtons(input);
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

void TraktorF1MK2::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  m_buttonStates[static_cast<unsigned>(Button::Shift)] = shiftPressed;

  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kF1MK2_buttonsDataSize - 1; i++) // Skip the last byte (encoder value)
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
          if (currentButton >= Button::Pad8 && currentButton <= Button::Pad9)
          {
            keyChanged(btn, buttonPressed ? 1.0 : 0.0, shiftPressed);
          }
          else
          {
            buttonChanged(changedButton, buttonPressed, shiftPressed);
          }
        }
      }
    }
  }

  // encoder
  uint8_t currentValue = input_.data()[kF1MK2_buttonsDataSize];
  if (currentValue != m_encoderValue)
  {
    bool valueIncreased = ((static_cast<uint8_t>(m_encoderValue) < currentValue)
                            || ((m_encoderValue == 0xff) && (currentValue == 0x00)))
                          && (!((m_encoderValue == 0x0) && (currentValue == 0xff)));
    m_encoderValue = currentValue;
    encoderChanged(0, valueIncreased, shiftPressed);
  }

  // pots/faders
  for (uint8_t potIndex = 0, i = kF1MK2_buttonsDataSize + 1; potIndex < 8; i += 2, potIndex++)
  {
    unsigned value = (input_.data()[i]) | (input_.data()[i + 1] << 8);
    if (m_potentiometersValues[potIndex] != value)
    {
      m_potentiometersValues[potIndex] = value;
      controlChanged(potIndex, value / 1024.0, shiftPressed);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void TraktorF1MK2::setLedImpl(Led led_, const Color& color_)
{
  unsigned ledIndex = static_cast<unsigned>(led_);

  if (Led::Unknown == led_)
  {
    return;
  }

  if (isRGBLed(led_))
  {
    uint8_t currentB = m_leds[ledIndex];
    uint8_t currentR = m_leds[ledIndex + 1];
    uint8_t currentG = m_leds[ledIndex + 2];

    m_leds[ledIndex] = 0x7F & color_.blue();
    m_leds[ledIndex + 1] = 0x7F & color_.red();
    m_leds[ledIndex + 2] = 0x7F & color_.green();

    m_isDirtyLeds = m_isDirtyLeds || (currentR != color_.red() || currentG != color_.green()
                                       || currentB != color_.blue());
  }
  else
  {
    uint8_t currentVal = m_leds[ledIndex];
    uint8_t newVal = 0x7F & color_.mono();

    m_leds[ledIndex] = newVal;
    m_isDirtyLeds = m_isDirtyLeds || (currentVal != newVal);
  }
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::isRGBLed(Led led_) const noexcept
{
  if (led_ >= Led::Pad13 && led_ <= Led::Pad4)
  {
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

TraktorF1MK2::Led TraktorF1MK2::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Browse);
    M_LED_CASE(Size);
    M_LED_CASE(Type);
    M_LED_CASE(Reverse);
    M_LED_CASE(Shift);
    M_LED_CASE(Capture);
    M_LED_CASE(Quant);
    M_LED_CASE(Sync);
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
    M_LED_CASE(Stop4);
    M_LED_CASE(Stop3);
    M_LED_CASE(Stop2);
    M_LED_CASE(Stop1);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

TraktorF1MK2::Led TraktorF1MK2::led(unsigned index_) const noexcept
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

Device::Button TraktorF1MK2::deviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(Pad8);
    M_BTN_CASE(Pad7);
    M_BTN_CASE(Pad6);
    M_BTN_CASE(Pad5);
    M_BTN_CASE(Pad4);
    M_BTN_CASE(Pad3);
    M_BTN_CASE(Pad2);
    M_BTN_CASE(Pad1);

    M_BTN_CASE(Pad16);
    M_BTN_CASE(Pad15);
    M_BTN_CASE(Pad14);
    M_BTN_CASE(Pad13);
    M_BTN_CASE(Pad12);
    M_BTN_CASE(Pad11);
    M_BTN_CASE(Pad10);
    M_BTN_CASE(Pad9);

    M_BTN_CASE(MainEncoder);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Size);
    M_BTN_CASE(Type);
    M_BTN_CASE(Reverse);
    M_BTN_CASE(Shift);

    M_BTN_CASE(Capture);
    M_BTN_CASE(Quant);
    M_BTN_CASE(Sync);
    M_BTN_CASE(Stop4);
    M_BTN_CASE(Stop3);
    M_BTN_CASE(Stop2);
    M_BTN_CASE(Stop1);

    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool TraktorF1MK2::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
