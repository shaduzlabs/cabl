/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineMK1.h"

#include <thread>

#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"

#include "cabl/gfx/TextDisplay.h"
#include "gfx/displays/NullCanvas.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kMASMK1_epDisplay = 0x08;
const uint8_t kMASMK1_epOut = 0x01;
const uint8_t kMASMK1_epInputPads = 0x84;
const uint8_t kMASMK1_epInputButtonsAndDials = 0x81;
const uint8_t kMASMK1_defaultDisplaysBacklight = 0x5C;
const unsigned kMASMK1_padThreshold = 200;
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

enum class MaschineMK1::Led : uint8_t
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
  PadMode,
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

//--------------------------------------------------------------------------------------------------

enum class MaschineMK1::Button : uint8_t
{
  Mute,
  Solo,
  Select,
  Duplicate,
  Navigate,
  PadMode,
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

//--------------------------------------------------------------------------------------------------

MaschineMK1::MaschineMK1()
{
  m_leds.resize(kMASMK1_ledsDataSize);
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::sendMidiMsg(tRawData midiMsg_)
{
  uint8_t lengthH = (midiMsg_.size() >> 8) & 0xFF;
  uint8_t lengthL = midiMsg_.size() & 0xFF;
  writeToDeviceHandle(
    Transfer({0x07, lengthH, lengthL}, midiMsg_.data(), midiMsg_.size()), kMASMK1_epOut);
}

//--------------------------------------------------------------------------------------------------

Canvas* MaschineMK1::graphicDisplay(size_t displayIndex_)
{
  static NullCanvas s_dummyDisplay;
  if (displayIndex_ > 1)
  {
    return &s_dummyDisplay;
  }

  return &m_displays[displayIndex_];
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::tick()
{
  static int state = 0;
  bool success = true;

  if (state == 0)
  {
    for (uint8_t displayIndex = 0; displayIndex < 2; displayIndex++)
    {
      if (m_displays[displayIndex].dirty())
      {
        success = sendFrame(displayIndex);
        m_displays[displayIndex].resetDirtyFlags();
      }
    }
  }
  else if (state == 1)
  {
    success = read();
  }
  else if (state == 2)
  {
    success = sendLeds();
  }

  if (!success)
  {
    std::string strStepName(state == 0 ? "sendFrame" : (state == 1 ? "read" : "sendLeds"));
    M_LOG("[MaschineMK1] tick: error in step #" << state << " (" << strStepName << ")");
  }

  if (++state >= 3)
  {
    state = 0;
  }
  return success;
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::init()
{
  // Displays
  for (int i = 0; i < kMASMK1_nDisplays; i++)
  {
    initDisplay(i);
    m_displays[i].black();
  }
  sendFrame(0);
  sendFrame(1);

  writeToDeviceHandle(Transfer({0x0B, 0xFF, 0x02, 0x05}), kMASMK1_epOut);

  std::fill(m_leds.begin(), m_leds.end(), 0);
  m_isDirtyLedGroup0 = true;
  m_isDirtyLedGroup1 = true;
  sendLeds();

  readFromDeviceHandleAsync(
    kMASMK1_epInputButtonsAndDials, std::bind(&MaschineMK1::cbRead, this, std::placeholders::_1));

  m_leds[static_cast<uint8_t>(Led::DisplayBacklight)] = kMASMK1_defaultDisplaysBacklight;
  m_isDirtyLedGroup1 = true;
  sendLeds();
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::initDisplay(uint8_t displayIndex_)
{
  if (displayIndex_ > 1)
  {
    return;
  }

  uint8_t d = displayIndex_ << 1;

  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x30}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x04, 0xCA, 0x04, 0x0F, 0x00}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x02, 0xBB, 0x00}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0xD1}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x94}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x81, 0x1E, 0x02}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x02, 0x20, 0x08}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x02, 0x20, 0x0B}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0xA6}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x31}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x04, 0x32, 0x00, 0x00, 0x05}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x34}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x30}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x04, 0xBC, 0x00, 0x01, 0x02}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x75, 0x00, 0x3F}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x15, 0x00, 0x54}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x5C}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0x25}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0xAF}), kMASMK1_epDisplay);

  std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Sleep 20 ms

  writeToDeviceHandle(Transfer({d, 0x00, 0x04, 0xBC, 0x02, 0x01, 0x01}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x01, 0xA6}), kMASMK1_epDisplay);

  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x81, 0x25, 0x02}), kMASMK1_epDisplay);
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::sendFrame(uint8_t displayIndex_)
{
  if (displayIndex_ > 1)
  {
    return false;
  }

  uint8_t d = displayIndex_ << 1;
  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x75, 0x00, 0x3F}), kMASMK1_epDisplay);
  writeToDeviceHandle(Transfer({d, 0x00, 0x03, 0x15, 0x00, 0x54}), kMASMK1_epDisplay);

  unsigned offset = 0;
  const unsigned dataSize = 502;

  if (!writeToDeviceHandle(
        Transfer({d, 0x01, 0xF7, 0x5C}, m_displays[displayIndex_].buffer() + offset, dataSize),
        kMASMK1_epDisplay))
  {
    return false;
  }

  d++;
  for (uint8_t chunk = 1; chunk < m_displays[displayIndex_].numberOfChunks() - 1; chunk++)
  {
    offset += dataSize;
    if (!writeToDeviceHandle(
          Transfer({d, 0x01, 0xF6}, m_displays[displayIndex_].buffer() + offset, dataSize),
          kMASMK1_epDisplay))
    {
      return false;
    }
  }

  offset += dataSize;

  if (!writeToDeviceHandle(
        Transfer({d, 0x01, 0x52}, m_displays[displayIndex_].buffer() + offset, 338),
        kMASMK1_epDisplay))
  {
    return false;
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::sendLeds()
{
  if (m_isDirtyLedGroup0)
  {
    if (!writeToDeviceHandle(Transfer({0x0C, 0x00}, &m_leds[0], 31), kMASMK1_epOut))
    {
      M_LOG("[MaschineMK1] sendLeds: error writing first block of leds");
      return false;
    }
    m_isDirtyLedGroup0 = false;
  }

  if (m_isDirtyLedGroup1)
  {
    if (!writeToDeviceHandle(Transfer({0x0C, 0x1E}, &m_leds[31], 31), kMASMK1_epOut))
    {
      M_LOG("[MaschineMK1] sendLeds: error writing second block of leds");
      return false;
    }
    m_isDirtyLedGroup1 = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::read()
{

  Transfer input;
  if (!readFromDeviceHandle(input, kMASMK1_epInputPads))
  {
    M_LOG("[MaschineMK1] read: ERROR");
    return false;
  }

  if (input[0] != 2) // Strange but I had to add this filter to avoid strange pad updates when turning encoders.
  {
    processPads(input);
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::processPads(const Transfer& input_)
{
  for (int i = 1; i < kMASMK1_padDataSize - 1; i += 2)
  {
    unsigned h = input_[i];
    unsigned l = input_[i - 1];
    uint8_t pad = (h & 0xF0) >> 4;

    m_padsData[pad] = (((h & 0x0F) << 8) | l);

    if (m_padsData[pad] > kMASMK1_padThreshold)
    {
      m_padsStatus[pad] = true;
      keyChanged(
        pad, m_padsData[pad] / 4096.0 , m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
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

void MaschineMK1::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  m_buttonStates[static_cast<unsigned>(Button::Shift)] = shiftPressed;

  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);
  if ((input_.data()[6] & 0x40) == 0)
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
        changedButton = deviceButton(currentButton);
        if (changedButton != Device::Button::Unknown)
        {
          //    std::copy(&input_[1],&input_[kMikroMK2_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
        }
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::processEncoders(const Transfer& input_)
{
  for (uint8_t i = 0; i < kMASMK1_nEncoders; i++)
  {
    unsigned currentEncValue = (input_.data()[2 + (2 * i)]) | (input_.data()[1 + (2 * i)] << 8);

    bool valueIncreased = true;

    uint8_t x = input_.data()[1 + (2 * i)];
    uint8_t y = input_.data()[2 + (2 * i)];
    uint8_t prevX = (m_encoderValues[i] >> 8) & 0xFF;
    uint8_t prevY = (m_encoderValues[i] & 0xFF);

    if (m_encoderValues[i] == currentEncValue)
    {
      continue;
    }

    if (x > 127)
    {
      valueIncreased = (y > 127) ? (x < prevX && y >= prevY) : (x >= prevX && y >= prevY);
    }
    else
    {
      valueIncreased = (y > 127) ? (x < prevX && y < prevY) : (x >= prevX && y < prevY);
    }

    if (m_encodersInitialized)
    {
#define M_ENCODER_CASE(val, index)                                                              \
  case val:                                                                                     \
    encoderChanged(index, valueIncreased, m_buttonStates[static_cast<uint8_t>(Button::Shift)]); \
    break

      switch (i)
      {
        M_ENCODER_CASE(0, 8);
        M_ENCODER_CASE(1, 4);
        M_ENCODER_CASE(2, 10);
        M_ENCODER_CASE(3, 7);
        M_ENCODER_CASE(4, 3);
        M_ENCODER_CASE(5, 9);
        M_ENCODER_CASE(6, 6);
        M_ENCODER_CASE(7, 2);
        M_ENCODER_CASE(8, 0);
        M_ENCODER_CASE(9, 5);
        M_ENCODER_CASE(10, 1);

        default:
        {
        }
      }
#undef M_ENCODER_CASE
    }
    m_encoderValues[i] = currentEncValue;
  }
  m_encodersInitialized = true;
}

//--------------------------------------------------------------------------------------------------

void MaschineMK1::setLedImpl(Led led_, const Color& color_)
{
  uint8_t ledIndex = static_cast<uint8_t>(led_);

  if (Led::Unknown == led_)
  {
    return;
  }

  uint8_t currentVal = m_leds[ledIndex];

  m_leds[ledIndex] = color_.mono();
  if (led_ > Led::Unused1)
  {
    m_isDirtyLedGroup1 = m_isDirtyLedGroup1 || (m_leds[ledIndex] != currentVal);
  }
  else
  {
    m_isDirtyLedGroup0 = m_isDirtyLedGroup0 || (m_leds[ledIndex] != currentVal);
  }
}

//--------------------------------------------------------------------------------------------------

MaschineMK1::Led MaschineMK1::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Mute);
    M_LED_CASE(Solo);
    M_LED_CASE(Select);
    M_LED_CASE(Duplicate);
    M_LED_CASE(Navigate);
    M_LED_CASE(PadMode);
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

//--------------------------------------------------------------------------------------------------

MaschineMK1::Led MaschineMK1::led(unsigned index_) const noexcept
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

Device::Button MaschineMK1::deviceButton(Button btn_) const noexcept
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
    M_BTN_CASE(PadMode);
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

//--------------------------------------------------------------------------------------------------

void MaschineMK1::cbRead(Transfer input_)
{
  if (input_[0] == 0x02)
  {
    processEncoders(input_);
  }
  else if (input_[0] == 0x04)
  {
    processButtons(input_);
  }
  else if (input_[0] == 0x06)
  {
    M_LOG("[MaschineMK1] read: received MIDI message");
    //!\todo Add MIDI in parsing
  }
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK1::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
