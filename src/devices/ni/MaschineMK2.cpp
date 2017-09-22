/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineMK2.h"

#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"
#include <thread>

#include "cabl/gfx/TextDisplay.h"
#include "gfx/displays/NullCanvas.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kMASMK2_epDisplay = 0x08;
const uint8_t kMASMK2_epOut = 0x01;
const uint8_t kMASMK2_epInput = 0x84;
const std::string kMASMK2_midiOutName = "Maschine Controller MK2";
const unsigned kMASMK2_padThreshold = 200;
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

// clang-format off
enum class MaschineMK2::Led : uint8_t{
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

  Restart = 48,
  TransportLeft,
  TransportRight,
  Grid,
  Play,
  Rec,
  Erase,
  Shift,

  Pad13, Pad13R = Pad13, Pad13G, Pad13B,
  Pad14, Pad14R = Pad14, Pad14G, Pad14B,
  Pad15, Pad15R = Pad15, Pad15G, Pad15B,
  Pad16, Pad16R = Pad16, Pad16G, Pad16B,
  Pad9,  Pad9R  = Pad9,  Pad9G,  Pad9B,
  Pad10, Pad10R = Pad10, Pad10G, Pad10B,
  Pad11, Pad11R = Pad11, Pad11G, Pad11B,
  Pad12, Pad12R = Pad12, Pad12G, Pad12B,
  Pad5,  Pad5R  = Pad5,  Pad5G,  Pad5B,
  Pad6,  Pad6R  = Pad6,  Pad6G,  Pad6B,
  Pad7,  Pad7R  = Pad7,  Pad7G,  Pad7B,
  Pad8,  Pad8R  = Pad8,  Pad8G,  Pad8B,
  Pad1,  Pad1R  = Pad1,  Pad1G,  Pad1B,
  Pad2,  Pad2R  = Pad2,  Pad2G,  Pad2B,
  Pad3,  Pad3R  = Pad3,  Pad3G,  Pad3B,
  Pad4,  Pad4R  = Pad4,  Pad4G,  Pad4B,

  GroupA, GroupAR1 = GroupA, GroupAG1, GroupAB1, GroupAR2, GroupAG2, GroupAB2,
  GroupB, GroupBR1 = GroupB, GroupBG1, GroupBB1, GroupBR2, GroupBG2, GroupBB2,
  GroupC, GroupCR1 = GroupC, GroupCG1, GroupCB1, GroupCR2, GroupCG2, GroupCB2,
  GroupD, GroupDR1 = GroupD, GroupDG1, GroupDB1, GroupDR2, GroupDG2, GroupDB2,
  GroupE, GroupER1 = GroupE, GroupEG1, GroupEB1, GroupER2, GroupEG2, GroupEB2,
  GroupF, GroupFR1 = GroupF, GroupFG1, GroupFB1, GroupFR2, GroupFG2, GroupFB2,
  GroupG, GroupGR1 = GroupG, GroupGG1, GroupGB1, GroupGR2, GroupGG2, GroupGB2,
  GroupH, GroupHR1 = GroupH, GroupHG1, GroupHB1, GroupHR2, GroupHG2, GroupHB2,

  Unknown,
};
// clang-format on

//--------------------------------------------------------------------------------------------------

enum class MaschineMK2::Button : uint8_t
{
  DisplayButton1,
  DisplayButton2,
  DisplayButton3,
  DisplayButton4,
  DisplayButton5,
  DisplayButton6,
  DisplayButton7,
  DisplayButton8,

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
  Main,

  GroupA,
  GroupB,
  GroupC,
  GroupD,
  GroupE,
  GroupF,
  GroupG,
  GroupH,

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
  Navigate,
  Duplicate,
  Select,
  Solo,
  Mute,

  NotUsed1,
  NotUsed2,
  NotUsed3,
  NotUsed4,
  NotUsed5,
  NotUsed6,
  NotUsed7,
  NotUsed8,

};

//--------------------------------------------------------------------------------------------------

MaschineMK2::MaschineMK2()
  : m_isDirtyPadLeds(false)
  , m_isDirtyGroupLeds(false)
  , m_isDirtyButtonLeds(false)
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  , m_pMidiout(new RtMidiOut)
#endif
{
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  std::string portName;
  unsigned nPorts = m_pMidiout->getPortCount();
  for (unsigned int i = 0; i < nPorts; i++)
  {
    try
    {
      portName = m_pMidiout->getPortName(i);
      if (portName == kMASMK2_midiOutName)
      {
        m_pMidiout->openPort(i);
      }
    }
    catch (RtMidiError& error)
    {
      std::string strError(error.getMessage());
      M_LOG("[MaschineMK2] RtMidiError: " << strError);
    }
  }
  if (!m_pMidiout->isPortOpen())
  {
    m_pMidiout.reset(nullptr);
  }
#endif
}

//--------------------------------------------------------------------------------------------------

MaschineMK2::~MaschineMK2()
{
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::sendMidiMsg(tRawData midiMsg_)
{
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  if (m_pMidiout)
  {
    m_pMidiout->sendMessage(&midiMsg_);
  }
#endif
}

//--------------------------------------------------------------------------------------------------

Canvas* MaschineMK2::graphicDisplay(size_t displayIndex_)
{
  static NullCanvas s_dummyDisplay;
  if (displayIndex_ > 1)
  {
    return &s_dummyDisplay;
  }

  return &m_displays[displayIndex_];
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::tick()
{
  static int state = 0;
  bool success = false;

  if (state == 0)
  {
    for (uint8_t displayIndex = 0; displayIndex < 2; displayIndex++)
    {
      if (m_displays[displayIndex].dirty())
      {
        success = sendFrame(displayIndex);
        m_displays[displayIndex].resetDirtyFlags();
      }
      else
      {
        success = true;
      }
    }
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

void MaschineMK2::init()
{
  // Display
  initDisplay();
  m_displays[0].white();
  m_displays[1].white();

  // Leds
  std::fill(std::begin(m_ledsButtons), std::end(m_ledsButtons), 0);
  std::fill(std::begin(m_ledsGroups), std::end(m_ledsGroups), 0);
  std::fill(std::begin(m_ledsPads), std::end(m_ledsPads), 0);
  m_isDirtyButtonLeds = true;
  m_isDirtyGroupLeds = true;
  m_isDirtyPadLeds = true;
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::initDisplay() const
{
  //!\todo set backlight
  return;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::sendFrame(uint8_t displayIndex_)
{
  if (displayIndex_ > 1)
  {
    return false;
  }

  uint8_t chunkByte = 0;
  for (uint8_t chunk = 0; chunk < 8; chunk++)
  {
    uint8_t firstByte = 0xE0 | displayIndex_;
    chunkByte = chunk * 8;
    const uint8_t* ptr = m_displays[displayIndex_].buffer() + (chunk * 256);
    if (!writeToDeviceHandle(
          Transfer({firstByte, 0x00, 0x00, chunkByte, 0x00, 0x20, 0x00, 0x08, 0x00}, ptr, 256),
          kMASMK2_epDisplay))
    {
      return false;
    }
  }

  m_displays[displayIndex_].resetDirtyFlags();
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::sendLeds()
{
  if (m_isDirtyButtonLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x82}, &m_ledsButtons[0], 32), kMASMK2_epOut))
    {
      return false;
    }
    m_isDirtyButtonLeds = false;
  }
  if (m_isDirtyGroupLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x81}, &m_ledsGroups[0], 57), kMASMK2_epOut))
    {
      return false;
    }
    m_isDirtyGroupLeds = false;
  }
  if (m_isDirtyPadLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x80}, &m_ledsPads[0], 49), kMASMK2_epOut))
    {
      return false;
    }
    m_isDirtyPadLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::read()
{
  Transfer input;
  for (uint8_t n = 0; n < 32; n++)
  {
    if (!readFromDeviceHandle(input, kMASMK2_epInput))
    {
      return false;
    }
    else if (input && input[0] == 0x01)
    {
      processButtons(input);
      return true;
    }
    else if (input && input[0] == 0x20 && n % 8 == 0) // Too many pad messages, need to skip some...
    {
      processPads(input);
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  m_buttonStates[static_cast<unsigned>(Button::Shift)] = shiftPressed;

  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kMASMK2_buttonsDataSize; i++) // Skip the last byte (encoder value)
  {
    for (int k = 0; k < 8; k++)
    {
      uint8_t btn = (i * 8) + k;
      Button currentButton(static_cast<Button>(btn));
      if (currentButton == Button::Shift || currentButton > Button::Mute)
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
          //    std::copy(&input_[1],&input_[kMASMK2_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
          return;
        }
      }
    }
  }

  // Now process the encoder data
  uint8_t currValue = input_.data()[kMASMK2_buttonsDataSize];
  if (currValue != m_encoderValues[0])
  {
    bool valueIncreased
      = ((m_encoderValues[0] < currValue) || ((m_encoderValues[0] == 0x0f) && (currValue == 0x00)))
        && (!((m_encoderValues[0] == 0x0) && (currValue == 0x0f)));
    m_encoderValues[0] = currValue;
    encoderChanged(0, valueIncreased, shiftPressed);
  }

  for (uint8_t encIndex = 0, i = kMASMK2_buttonsDataSize + 1; encIndex < 8; i += 2, encIndex++)
  {
    unsigned value = (input_.data()[i]) | (input_.data()[i + 1] << 8);
    unsigned hValue = input_.data()[i + 1];
    if (m_encoderValues[encIndex + 1] != value)
    {
      unsigned prevHValue = (m_encoderValues[encIndex + 1] & 0xF00) >> 8;
      bool valueIncreased
        = ((m_encoderValues[encIndex + 1] < value) || ((prevHValue == 3) && (hValue == 0)))
          && (!((prevHValue == 0) && (hValue == 3)));
      m_encoderValues[encIndex + 1] = value;
      encoderChanged(encIndex + 1, valueIncreased, shiftPressed);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void MaschineMK2::processPads(const Transfer& input_)
{
  //!\todo process pad data
  for (int i = 1; i < kMASMK2_padDataSize; i += 2)
  {
    unsigned l = input_[i];
    unsigned h = input_[i + 1];
    uint8_t pad = (h & 0xF0) >> 4;
    m_padsData[pad] = (((h & 0x0F) << 8) | l);

    if (m_padsData[pad] > kMASMK2_padThreshold)
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

void MaschineMK2::setLedImpl(Led led_, const Color& color_)
{
  static const uint8_t kFirstPadIndex = static_cast<uint8_t>(Led::Pad13);
  uint8_t ledIndex = static_cast<uint8_t>(led_);
  if (Led::Unknown == led_)
  {
    return;
  }
  if (isRGBLed(led_))
  {
    if (led_ < Led::GroupA)
    {
      uint8_t currentR = m_ledsPads[ledIndex - kFirstPadIndex];
      uint8_t currentG = m_ledsPads[ledIndex - kFirstPadIndex + 1];
      uint8_t currentB = m_ledsPads[ledIndex - kFirstPadIndex + 2];

      m_ledsPads[ledIndex - kFirstPadIndex] = color_.red();
      m_ledsPads[ledIndex - kFirstPadIndex + 1] = color_.green();
      m_ledsPads[ledIndex - kFirstPadIndex + 2] = color_.blue();
      m_isDirtyPadLeds = m_isDirtyPadLeds || (currentR != color_.red() || currentG != color_.green()
                                               || currentB != color_.blue());
    }
    else
    {
      uint8_t firstGroupIndex = static_cast<uint8_t>(Led::GroupA);
      uint8_t currentR = m_ledsGroups[ledIndex - firstGroupIndex];
      uint8_t currentG = m_ledsGroups[ledIndex - firstGroupIndex + 1];
      uint8_t currentB = m_ledsGroups[ledIndex - firstGroupIndex + 2];

      m_ledsGroups[ledIndex - firstGroupIndex] = color_.red();
      m_ledsGroups[ledIndex - firstGroupIndex + 1] = color_.green();
      m_ledsGroups[ledIndex - firstGroupIndex + 2] = color_.blue();
      m_ledsGroups[ledIndex - firstGroupIndex + 3] = color_.red();
      m_ledsGroups[ledIndex - firstGroupIndex + 4] = color_.green();
      m_ledsGroups[ledIndex - firstGroupIndex + 5] = color_.blue();

      m_isDirtyGroupLeds
        = m_isDirtyGroupLeds
          || (currentR != color_.red() || currentG != color_.green() || currentB != color_.blue());
    }
  }
  else
  {
    uint8_t currentVal = m_ledsGroups[ledIndex];
    uint8_t newVal = color_.mono();
    if (led_ >= Led::Restart)
    {
      m_ledsGroups[ledIndex] = newVal;
      m_isDirtyGroupLeds = (currentVal != newVal);
    }
    else
    {
      m_ledsButtons[ledIndex] = newVal;
      m_isDirtyButtonLeds = m_isDirtyButtonLeds || (currentVal != newVal);
    }
  }
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::isRGBLed(Led led_) const noexcept
{

  if (Led::GroupA == led_ || Led::GroupB == led_ || Led::GroupC == led_ || Led::GroupD == led_
      || Led::GroupE == led_
      || Led::GroupF == led_
      || Led::GroupG == led_
      || Led::GroupH == led_
      || Led::Pad1 == led_
      || Led::Pad2 == led_
      || Led::Pad3 == led_
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

MaschineMK2::Led MaschineMK2::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Control);
    M_LED_CASE(Step);
    M_LED_CASE(Browse);
    M_LED_CASE(Sampling);
    M_LED_CASE(BrowseLeft);
    M_LED_CASE(BrowseRight);
    M_LED_CASE(All);
    M_LED_CASE(AutoWrite);
    M_LED_CASE(DisplayButton1);
    M_LED_CASE(DisplayButton2);
    M_LED_CASE(DisplayButton3);
    M_LED_CASE(DisplayButton4);
    M_LED_CASE(DisplayButton5);
    M_LED_CASE(DisplayButton6);
    M_LED_CASE(DisplayButton7);
    M_LED_CASE(DisplayButton8);
    M_LED_CASE(Scene);
    M_LED_CASE(Pattern);
    M_LED_CASE(PadMode);
    M_LED_CASE(Navigate);
    M_LED_CASE(Duplicate);
    M_LED_CASE(Select);
    M_LED_CASE(Solo);
    M_LED_CASE(Mute);
    M_LED_CASE(Volume);
    M_LED_CASE(Swing);
    M_LED_CASE(Tempo);
    M_LED_CASE(MasterLeft);
    M_LED_CASE(MasterRight);
    M_LED_CASE(Enter);
    M_LED_CASE(NoteRepeat);
    M_LED_CASE(Restart);
    M_LED_CASE(TransportLeft);
    M_LED_CASE(TransportRight);
    M_LED_CASE(Grid);
    M_LED_CASE(Play);
    M_LED_CASE(Rec);
    M_LED_CASE(Erase);
    M_LED_CASE(Shift);
    M_LED_CASE(GroupA);
    M_LED_CASE(GroupB);
    M_LED_CASE(GroupC);
    M_LED_CASE(GroupD);
    M_LED_CASE(GroupE);
    M_LED_CASE(GroupF);
    M_LED_CASE(GroupG);
    M_LED_CASE(GroupH);

    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

MaschineMK2::Led MaschineMK2::led(unsigned index_) const noexcept
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

Device::Button MaschineMK2::deviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(DisplayButton1);
    M_BTN_CASE(DisplayButton2);
    M_BTN_CASE(DisplayButton3);
    M_BTN_CASE(DisplayButton4);
    M_BTN_CASE(DisplayButton5);
    M_BTN_CASE(DisplayButton6);
    M_BTN_CASE(DisplayButton7);
    M_BTN_CASE(DisplayButton8);
    M_BTN_CASE(Control);
    M_BTN_CASE(Step);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Sampling);
    M_BTN_CASE(BrowseLeft);
    M_BTN_CASE(BrowseRight);
    M_BTN_CASE(All);
    M_BTN_CASE(AutoWrite);
    M_BTN_CASE(Volume);
    M_BTN_CASE(Swing);
    M_BTN_CASE(Tempo);
    M_BTN_CASE(MasterLeft);
    M_BTN_CASE(MasterRight);
    M_BTN_CASE(Enter);
    M_BTN_CASE(NoteRepeat);
    M_BTN_CASE(GroupA);
    M_BTN_CASE(GroupB);
    M_BTN_CASE(GroupC);
    M_BTN_CASE(GroupD);
    M_BTN_CASE(GroupE);
    M_BTN_CASE(GroupF);
    M_BTN_CASE(GroupG);
    M_BTN_CASE(GroupH);
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
    M_BTN_CASE(Navigate);
    M_BTN_CASE(Duplicate);
    M_BTN_CASE(Select);
    M_BTN_CASE(Solo);
    M_BTN_CASE(Mute);
    M_BTN_CASE(Main);


    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool MaschineMK2::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
