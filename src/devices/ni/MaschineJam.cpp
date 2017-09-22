/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineJam.h"

#include <thread>

#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"

#include "cabl/gfx/LedArray.h"
#include "cabl/gfx/LedMatrix.h"
#include "cabl/gfx/TextDisplay.h"

#include "gfx/LedArrayDummy.h"
#include "gfx/displays/NullCanvas.h"

#include "devices/ni/MaschineJamHelper.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kMASJ_epOut = 0x01;
const uint8_t kMASJ_epInput = 0x84;
const std::string kMASJ_midiOutName = "Maschine Controller MK2";
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

// clang-format off
enum class MaschineJam::Led : uint8_t{
  // 0x80
  Arrange,
  Step,
  PadMode,
  Clear,
  Duplicate,
  NavigateUp,
  NavigateLeft,
  NavigateRight,

  NavigateDown,
  Arp,
  Mst,
  Grp,
  In,
  // none
  Cue = 14,
  // none

  Browse = 16,
  Macro,
  Level,
  Aux,
  Control,
  Auto,
  Perform,
  Variation,

  Lock,
  Tune,
  Swing,
  Shift,
  Play,
  Rec,
  TransportLeft,
  TransportRight,

  Tempo,
  Grid,
  Solo,
  Mute,
  Select,
  LevelLeft1,
  LevelLeft2,
  LevelLeft3,

  LevelLeft4,
  LevelLeft5,
  LevelLeft6,
  LevelLeft7,
  LevelLeft8,
  LevelRight1,
  LevelRight2,
  LevelRight3,

  LevelRight4,
  LevelRight5,
  LevelRight6,
  LevelRight7,
  LevelRight8,

  // 0x81
  DisplayButton1,
  DisplayButton2,
  DisplayButton3,
  DisplayButton4,
  DisplayButton5,
  DisplayButton6,
  DisplayButton7,
  DisplayButton8,

  Pad1,  Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
  Pad9,  Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16,
  Pad17, Pad18, Pad19, Pad20, Pad21, Pad22, Pad23, Pad24,
  Pad25, Pad26, Pad27, Pad28, Pad29, Pad30, Pad31, Pad32,
  Pad33, Pad34, Pad35, Pad36, Pad37, Pad38, Pad39, Pad40,
  Pad41, Pad42, Pad43, Pad44, Pad45, Pad46, Pad47, Pad48,
  Pad49, Pad50, Pad51, Pad52, Pad53, Pad54, Pad55, Pad56,
  Pad57, Pad58, Pad59, Pad60, Pad61, Pad62, Pad63, Pad64,

  GroupA, GroupB, GroupC, GroupD, GroupE, GroupF, GroupG, GroupH,

  // 0x82
  Strip1L1, Strip1L2, Strip1L3, Strip1L4, Strip1L5, Strip1L6, Strip1L7, Strip1L8, Strip1L9, Strip1L10, Strip1L11,
  Strip2L1, Strip2L2, Strip2L3, Strip2L4, Strip2L5, Strip2L6, Strip2L7, Strip2L8, Strip2L9, Strip2L10, Strip2L11,
  Strip3L1, Strip3L2, Strip3L3, Strip3L4, Strip3L5, Strip3L6, Strip3L7, Strip3L8, Strip3L9, Strip3L10, Strip3L11,
  Strip4L1, Strip4L2, Strip4L3, Strip4L4, Strip4L5, Strip4L6, Strip4L7, Strip4L8, Strip4L9, Strip4L10, Strip4L11,
  Strip5L1, Strip5L2, Strip5L3, Strip5L4, Strip5L5, Strip5L6, Strip5L7, Strip5L8, Strip5L9, Strip5L10, Strip5L11,
  Strip6L1, Strip6L2, Strip6L3, Strip6L4, Strip6L5, Strip6L6, Strip6L7, Strip6L8, Strip6L9, Strip6L10, Strip6L11,
  Strip7L1, Strip7L2, Strip7L3, Strip7L4, Strip7L5, Strip7L6, Strip7L7, Strip7L8, Strip7L9, Strip7L10, Strip7L11,
  Strip8L1, Strip8L2, Strip8L3, Strip8L4, Strip8L5, Strip8L6, Strip8L7, Strip8L8, Strip8L9, Strip8L10, Strip8L11,

  Unknown,
};
// clang-format on

//--------------------------------------------------------------------------------------------------

enum class MaschineJam::Button : uint8_t
{
  Arrange,
  DisplayButton1,
  DisplayButton2,
  DisplayButton3,
  DisplayButton4,
  DisplayButton5,
  DisplayButton6,
  DisplayButton7,

  DisplayButton8,
  Step,
  PadMode,
  Clear,
  Duplicate,
  NavigateUp,
  NavigateLeft,
  NavigateRight,

  NavigateDown,
  Arp,
  Pad1,
  Pad2,
  Pad3,
  Pad4,
  Pad5,
  Pad6,

  Pad7,
  Pad8,
  Pad9,
  Pad10,
  Pad11,
  Pad12,
  Pad13,
  Pad14,

  Pad15,
  Pad16,
  Pad17,
  Pad18,
  Pad19,
  Pad20,
  Pad21,
  Pad22,

  Pad23,
  Pad24,
  Pad25,
  Pad26,
  Pad27,
  Pad28,
  Pad29,
  Pad30,

  Pad31,
  Pad32,
  Pad33,
  Pad34,
  Pad35,
  Pad36,
  Pad37,
  Pad38,

  Pad39,
  Pad40,
  Pad41,
  Pad42,
  Pad43,
  Pad44,
  Pad45,
  Pad46,

  Pad47,
  Pad48,
  Pad49,
  Pad50,
  Pad51,
  Pad52,
  Pad53,
  Pad54,

  Pad55,
  Pad56,
  Pad57,
  Pad58,
  Pad59,
  Pad60,
  Pad61,
  Pad62,

  Pad63,
  Pad64,
  GroupA,
  GroupB,
  GroupC,
  GroupD,
  GroupE,
  GroupF,

  GroupG,
  GroupH,
  Mst,
  Grp,
  In,
  Cue,
  Browse,
  Macro,

  Level,
  Aux,
  Control,
  Auto,
  Perform,
  Variation,
  Lock,
  Tune,

  Swing,
  Shift,
  Play,
  Rec,
  TransportLeft,
  TransportRight,
  Tempo,
  Grid,

  Solo,
  Mute,
  Select,
  TouchEncoderMain,
  MainEncoder,
  NotUsed2,
  NotUsed3,
  NotUsed4,
};

//--------------------------------------------------------------------------------------------------

MaschineJam::MaschineJam()
{
}

//--------------------------------------------------------------------------------------------------

void MaschineJam::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void MaschineJam::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

Canvas* MaschineJam::ledMatrix(size_t ledMatrixIndex_)
{
  static NullCanvas s_dummyLedMatrix;
  if (ledMatrixIndex_ == 0)
  {
    return &m_ledMatrix;
  }

  return &s_dummyLedMatrix;
}

//--------------------------------------------------------------------------------------------------

LedArray* MaschineJam::ledArray(size_t ledArrayIndex_)
{
  static LedArrayDummy s_dummyLedArray;
  if (ledArrayIndex_ < 8)
  {
    return &m_ledArraysStrips[ledArrayIndex_];
  }
  else if (ledArrayIndex_ < 10)
  {
    return &m_ledArraysLevel[ledArrayIndex_ - 8];
  }

  return &s_dummyLedArray;
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::tick()
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

//--------------------------------------------------------------------------------------------------

void MaschineJam::init()
{
  // Leds
  std::fill(std::begin(m_ledsButtons), std::end(m_ledsButtons), 0);
  std::fill(std::begin(m_ledsStrips), std::end(m_ledsStrips), 0);
  std::fill(std::begin(m_ledsPads), std::end(m_ledsPads), 0);

  m_ledMatrix.resetDirtyFlags();
  m_isDirtyButtonLeds = true;
  m_isDirtyStripLeds = true;
  m_isDirtyPadLeds = true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::sendLeds()
{
  if (m_ledMatrix.dirty())
  {
    unsigned pixel = 8;
    for (unsigned y = 0; y < m_ledMatrix.height(); y++)
    {
      for (unsigned x = 0; x < m_ledMatrix.width(); x++)
      {
        m_ledsPads[pixel++] = MaschineJamHelper::toLedColor(m_ledMatrix.pixel(x, y));
      }
    }
    m_ledMatrix.resetDirtyFlags();
    m_isDirtyPadLeds = true;
  }
  for (unsigned i = 0; i < numOfLedArrays(); i++)
  {
    if (i < 8 && m_ledArraysStrips[i].dirty())
    {
      std::copy(m_ledArraysStrips[i].buffer(),
        m_ledArraysStrips[i].buffer() + m_ledArraysStrips[i].length(),
        &m_ledsStrips[i * 11]);
      m_ledArraysStrips[i].resetDirty();
      m_isDirtyStripLeds = true;
    }
    else if ((i == 8 || i == 9) && m_ledArraysLevel[i - 8].dirty())
    {
      unsigned offset = (i == 8 ? static_cast<unsigned>(Led::LevelLeft1)
                                : static_cast<unsigned>(Led::LevelRight1));
      for (unsigned k = 0; k < m_ledArraysLevel[i - 8].length(); k++)
      {
        m_ledsButtons[offset + (k)]
          = MaschineJamHelper::fromLedColor(m_ledArraysLevel[i - 8].buffer()[k]).mono();
      }
      m_ledArraysLevel[i - 8].resetDirty();
      m_isDirtyButtonLeds = true;
    }
  }

  if (m_isDirtyButtonLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x80}, &m_ledsButtons[0], kMASJ_nLedsButtons), kMASJ_epOut))
    {
      return false;
    }
    m_isDirtyButtonLeds = false;
  }
  if (m_isDirtyPadLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x81}, &m_ledsPads[0], kMASJ_nLedsPads), kMASJ_epOut))
    {
      return false;
    }
    m_isDirtyPadLeds = false;
  }
  if (m_isDirtyStripLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x82}, &m_ledsStrips[0], kMASJ_nLedsStrips), kMASJ_epOut))
    {
      return false;
    }
    m_isDirtyStripLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::read()
{
  Transfer input;
  if (!readFromDeviceHandle(input, kMASJ_epInput))
  {
    return false;
  }
  else if (input && input[0] == 0x01)
  {
    processButtons(input);
    return true;
  }
  else if (input && input[0] == 0x02)
  {
    processStrips(input);
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

void MaschineJam::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  m_buttonStates[static_cast<unsigned>(Button::Shift)] = shiftPressed;

  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kMASJ_buttonsDataSize - 2; i++) // Skip the first byte (encoder value)
  {
    for (int k = 0; k < 8; k++)
    {
      uint8_t btn = (i * 8) + k;
      Button currentButton(static_cast<Button>(btn));
      if (currentButton == Button::Shift || currentButton > Button::MainEncoder)
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
          if (currentButton >= Button::Pad1 && currentButton <= Button::Pad64)
          {
            unsigned padIndex
              = static_cast<unsigned>(currentButton) - static_cast<unsigned>(Button::Pad1);
            keyChanged(padIndex, buttonPressed ? 1.0 : 0.0, shiftPressed);
          }
          else
          {
            buttonChanged(changedButton, buttonPressed, shiftPressed);
          }
          return;
        }
      }
    }
  }

  // Now process the encoder data
  uint8_t currentEncoderValue = input_.data()[1];
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

void MaschineJam::processStrips(const Transfer& input_)
{
  for (unsigned i = 1, tsIndex = 0; i < input_.size(); i += 6, tsIndex++)
  {
    // unsigned timeMs = input_[i] | (input_[i + 1] << 8);
    unsigned val = input_[i + 2] | (input_[i + 3] << 8);

    if (val != 0 && m_touchstripsValues[tsIndex] != val)
    {
      m_touchstripsValues[tsIndex] = val;
      controlChanged(tsIndex, val / 1024.0, m_buttonStates[static_cast<uint8_t>(Button::Shift)]);
    }
  }
}

//--------------------------------------------------------------------------------------------------

void MaschineJam::setLedImpl(Led led_, const Color& color_)
{
  unsigned ledIndex = static_cast<uint8_t>(led_);
  if (Led::Unknown == led_)
  {
    return;
  }

  if (led_ < Led::DisplayButton1)
  {
    // normal buttons
    uint8_t currentVal = m_ledsButtons[ledIndex];
    uint8_t newVal = color_.mono();
    m_ledsButtons[ledIndex] = newVal;
    m_isDirtyButtonLeds = m_isDirtyButtonLeds || (currentVal != newVal);
  }
  else if (led_ < Led::Strip1L1)
  {
    // matrix buttons, pads and groups
    ledIndex -= static_cast<unsigned>(Led::DisplayButton1);
    uint8_t currentVal = m_ledsPads[ledIndex];
    uint8_t newVal = MaschineJamHelper::toLedColor(color_);
    m_ledsPads[ledIndex] = newVal;
    m_isDirtyPadLeds = m_isDirtyPadLeds || (currentVal != newVal);
  }
  else
  {
    // touch strips
    ledIndex -= static_cast<unsigned>(Led::Strip1L1);
    uint8_t currentVal = m_ledsStrips[ledIndex];
    uint8_t newVal = MaschineJamHelper::toLedColor(color_);
    m_ledsStrips[ledIndex] = newVal;
    m_isDirtyStripLeds = m_isDirtyStripLeds || (currentVal != newVal);
  }
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::isRGBLed(Led led_) const noexcept
{

  if (led_ >= Led::DisplayButton1)
  {
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

MaschineJam::Led MaschineJam::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Arrange);
    M_LED_CASE(Step);
    M_LED_CASE(PadMode);
    M_LED_CASE(Clear);
    M_LED_CASE(Duplicate);
    M_LED_CASE(NavigateUp);
    M_LED_CASE(NavigateLeft);
    M_LED_CASE(NavigateRight);
    M_LED_CASE(NavigateDown);
    M_LED_CASE(Arp);
    M_LED_CASE(Mst);
    M_LED_CASE(Grp);
    M_LED_CASE(In);
    M_LED_CASE(Cue);
    M_LED_CASE(Browse);
    M_LED_CASE(Macro);
    M_LED_CASE(Level);
    M_LED_CASE(Aux);
    M_LED_CASE(Control);
    M_LED_CASE(Auto);
    M_LED_CASE(Perform);
    M_LED_CASE(Variation);
    M_LED_CASE(Lock);
    M_LED_CASE(Tune);
    M_LED_CASE(Swing);
    M_LED_CASE(Shift);
    M_LED_CASE(Play);
    M_LED_CASE(Rec);
    M_LED_CASE(TransportLeft);
    M_LED_CASE(TransportRight);
    M_LED_CASE(Tempo);
    M_LED_CASE(Grid);
    M_LED_CASE(Solo);
    M_LED_CASE(Mute);
    M_LED_CASE(Select);
    M_LED_CASE(DisplayButton1);
    M_LED_CASE(DisplayButton2);
    M_LED_CASE(DisplayButton3);
    M_LED_CASE(DisplayButton4);
    M_LED_CASE(DisplayButton5);
    M_LED_CASE(DisplayButton6);
    M_LED_CASE(DisplayButton7);
    M_LED_CASE(DisplayButton8);
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
    M_LED_CASE(Pad17);
    M_LED_CASE(Pad18);
    M_LED_CASE(Pad19);
    M_LED_CASE(Pad20);
    M_LED_CASE(Pad21);
    M_LED_CASE(Pad22);
    M_LED_CASE(Pad23);
    M_LED_CASE(Pad24);
    M_LED_CASE(Pad25);
    M_LED_CASE(Pad26);
    M_LED_CASE(Pad27);
    M_LED_CASE(Pad28);
    M_LED_CASE(Pad29);
    M_LED_CASE(Pad30);
    M_LED_CASE(Pad31);
    M_LED_CASE(Pad32);
    M_LED_CASE(Pad33);
    M_LED_CASE(Pad34);
    M_LED_CASE(Pad35);
    M_LED_CASE(Pad36);
    M_LED_CASE(Pad37);
    M_LED_CASE(Pad38);
    M_LED_CASE(Pad39);
    M_LED_CASE(Pad40);
    M_LED_CASE(Pad41);
    M_LED_CASE(Pad42);
    M_LED_CASE(Pad43);
    M_LED_CASE(Pad44);
    M_LED_CASE(Pad45);
    M_LED_CASE(Pad46);
    M_LED_CASE(Pad47);
    M_LED_CASE(Pad48);
    M_LED_CASE(Pad49);
    M_LED_CASE(Pad50);
    M_LED_CASE(Pad51);
    M_LED_CASE(Pad52);
    M_LED_CASE(Pad53);
    M_LED_CASE(Pad54);
    M_LED_CASE(Pad55);
    M_LED_CASE(Pad56);
    M_LED_CASE(Pad57);
    M_LED_CASE(Pad58);
    M_LED_CASE(Pad59);
    M_LED_CASE(Pad60);
    M_LED_CASE(Pad61);
    M_LED_CASE(Pad62);
    M_LED_CASE(Pad63);
    M_LED_CASE(Pad64);
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

MaschineJam::Led MaschineJam::led(unsigned index_) const noexcept
{
  unsigned index = static_cast<unsigned>(Led::Pad1) + index_;
  if (index_ < 64)
  {
    return static_cast<Led>(index);
  }

  return Led::Unknown;
}

//--------------------------------------------------------------------------------------------------

Device::Button MaschineJam::deviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(Arrange);
    M_BTN_CASE(Step);
    M_BTN_CASE(PadMode);
    M_BTN_CASE(Clear);
    M_BTN_CASE(Duplicate);
    M_BTN_CASE(NavigateUp);
    M_BTN_CASE(NavigateLeft);
    M_BTN_CASE(NavigateRight);
    M_BTN_CASE(NavigateDown);
    M_BTN_CASE(Arp);
    M_BTN_CASE(Mst);
    M_BTN_CASE(Grp);
    M_BTN_CASE(In);
    M_BTN_CASE(Cue);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Macro);
    M_BTN_CASE(Level);
    M_BTN_CASE(Aux);
    M_BTN_CASE(Control);
    M_BTN_CASE(Auto);
    M_BTN_CASE(Perform);
    M_BTN_CASE(Variation);
    M_BTN_CASE(Lock);
    M_BTN_CASE(Tune);
    M_BTN_CASE(Swing);
    M_BTN_CASE(Shift);
    M_BTN_CASE(Play);
    M_BTN_CASE(Rec);
    M_BTN_CASE(TransportLeft);
    M_BTN_CASE(TransportRight);
    M_BTN_CASE(Tempo);
    M_BTN_CASE(Grid);
    M_BTN_CASE(Solo);
    M_BTN_CASE(Mute);
    M_BTN_CASE(Select);
    M_BTN_CASE(TouchEncoderMain);
    M_BTN_CASE(MainEncoder);
    M_BTN_CASE(DisplayButton1);
    M_BTN_CASE(DisplayButton2);
    M_BTN_CASE(DisplayButton3);
    M_BTN_CASE(DisplayButton4);
    M_BTN_CASE(DisplayButton5);
    M_BTN_CASE(DisplayButton6);
    M_BTN_CASE(DisplayButton7);
    M_BTN_CASE(DisplayButton8);
    M_BTN_CASE(Pad1);
    M_BTN_CASE(Pad2);
    M_BTN_CASE(Pad3);
    M_BTN_CASE(Pad4);
    M_BTN_CASE(Pad5);
    M_BTN_CASE(Pad6);
    M_BTN_CASE(Pad7);
    M_BTN_CASE(Pad8);
    M_BTN_CASE(Pad9);
    M_BTN_CASE(Pad10);
    M_BTN_CASE(Pad11);
    M_BTN_CASE(Pad12);
    M_BTN_CASE(Pad13);
    M_BTN_CASE(Pad14);
    M_BTN_CASE(Pad15);
    M_BTN_CASE(Pad16);
    M_BTN_CASE(Pad17);
    M_BTN_CASE(Pad18);
    M_BTN_CASE(Pad19);
    M_BTN_CASE(Pad20);
    M_BTN_CASE(Pad21);
    M_BTN_CASE(Pad22);
    M_BTN_CASE(Pad23);
    M_BTN_CASE(Pad24);
    M_BTN_CASE(Pad25);
    M_BTN_CASE(Pad26);
    M_BTN_CASE(Pad27);
    M_BTN_CASE(Pad28);
    M_BTN_CASE(Pad29);
    M_BTN_CASE(Pad30);
    M_BTN_CASE(Pad31);
    M_BTN_CASE(Pad32);
    M_BTN_CASE(Pad33);
    M_BTN_CASE(Pad34);
    M_BTN_CASE(Pad35);
    M_BTN_CASE(Pad36);
    M_BTN_CASE(Pad37);
    M_BTN_CASE(Pad38);
    M_BTN_CASE(Pad39);
    M_BTN_CASE(Pad40);
    M_BTN_CASE(Pad41);
    M_BTN_CASE(Pad42);
    M_BTN_CASE(Pad43);
    M_BTN_CASE(Pad44);
    M_BTN_CASE(Pad45);
    M_BTN_CASE(Pad46);
    M_BTN_CASE(Pad47);
    M_BTN_CASE(Pad48);
    M_BTN_CASE(Pad49);
    M_BTN_CASE(Pad50);
    M_BTN_CASE(Pad51);
    M_BTN_CASE(Pad52);
    M_BTN_CASE(Pad53);
    M_BTN_CASE(Pad54);
    M_BTN_CASE(Pad55);
    M_BTN_CASE(Pad56);
    M_BTN_CASE(Pad57);
    M_BTN_CASE(Pad58);
    M_BTN_CASE(Pad59);
    M_BTN_CASE(Pad60);
    M_BTN_CASE(Pad61);
    M_BTN_CASE(Pad62);
    M_BTN_CASE(Pad63);
    M_BTN_CASE(Pad64);
    M_BTN_CASE(GroupA);
    M_BTN_CASE(GroupB);
    M_BTN_CASE(GroupC);
    M_BTN_CASE(GroupD);
    M_BTN_CASE(GroupE);
    M_BTN_CASE(GroupF);
    M_BTN_CASE(GroupG);
    M_BTN_CASE(GroupH);

    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool MaschineJam::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[2 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
