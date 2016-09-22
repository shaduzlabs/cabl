/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/akai/Push.h"

#include <algorithm>
#include <limits>
#include <thread>

#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/gfx/TextDisplay.h"
#include "cabl/util/Functions.h"
#include "gfx/displays/NullCanvas.h"

#include <cmath>

//!\todo delete debug includes
#include <iomanip>
#include <iostream>

//--------------------------------------------------------------------------------------------------

namespace
{
const uint8_t kPush_epOut = 0x01;
const uint8_t kPush_manufacturerId = 0x47; // Akai manufacturer Id

// clang-format off
const std::vector<sl::cabl::Color> kPush_colors{
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
//| R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
  {   0,   0,   0 }, {  30,  30,  30 }, { 127, 127, 127 }, { 255, 255, 255 }, { 255,  76,  76 },
  { 255,   0,   0 }, {  89,   0,   0 }, {  25,   0,   0 }, { 255, 189, 108 }, { 255,  84,   0 },
  {  89,  29,   0 }, {  39,  27,   0 }, { 255, 255,  76 }, { 255, 255,   0 }, {  89,  89,   0 },
  {  25,  25,   0 }, { 136, 255,  76 }, {  84, 255,   0 }, {  29,  89,   0 }, {  20,  43,   0 },
  {  76, 255,  76 }, {   0, 255,   0 }, {   0,  89,   0 }, {   0,  25,   0 }, {  76, 255,  94 },
  {   0, 255,  25 }, {   0,  89,  13 }, {   0,  25,   2 }, {  76, 255, 136 }, {   0, 255,  85 },
  {   0,  89,  29 }, {   0,  31,  18 }, {  76, 255, 183 }, {   0, 255, 153 }, {   0,  89,  53 },
  {   0,  25,  18 }, {  76, 195, 255 }, {   0, 169, 255 }, {   0,  65,  82 }, {   0,  16,  25 },
  {  76, 136, 255 }, {   0,  85, 255 }, {   0,  29,  89 }, {   0,   8,  25 }, {  76,  76, 255 },
  {   0,   0, 255 }, {   0,   0,  89 }, {   0,   0,  25 }, { 135,  76, 255 }, {  84,   0, 255 },
  {  25,   0, 100 }, {  15,   0,  48 }, { 255,  76, 255 }, { 255,   0, 255 }, {  89,   0,  89 },
  {  25,   0,  25 }, { 255,  76, 135 }, { 255,   0,  84 }, {  89,   0,  29 }, {  34,   0,  19 },
  { 255,  21,   0 }, { 153,  53,   0 }, { 121,  81,   0 }, {  67, 100,   0 }, {   3,  57,   0 },
  {   0,  87,  53 }, {   0,  84, 127 }, {   0,   0, 255 }, {   0,  69,  79 }, {  37,   0, 204 },
  { 127, 127, 127 }, {  32,  32,  32 }, { 255,   0,   0 }, { 189, 255,  45 }, { 175, 237,   6 },
  { 100, 255,   9 }, {  16, 139,   0 }, {   0, 255, 135 }, {   0, 169, 255 }, {   0,  42, 255 },
  {  63,   0, 255 }, { 122,   0, 255 }, { 178,  26, 125 }, {  64,  33,   0 }, { 255,  74,   0 },
  { 136, 225,   6 }, { 114, 255,  21 }, {   0, 255,   0 }, {  59, 255,  38 }, {  89, 255, 113 },
  {  56, 255, 204 }, {  91, 138, 255 }, {  49,  81, 198 }, { 135, 127, 233 }, { 211,  29, 255 },
  { 255,   0,  93 }, { 255, 127,   0 }, { 185, 176,   0 }, { 144, 255,   0 }, { 131,  93,   7 },
  {  57,  43,   0 }, {  20,  76,  16 }, {  13,  80,  56 }, {  21,  21,  42 }, {  22,  32,  90 },
  { 105,  60,  28 }, { 168,   0,  10 }, { 222,  81,  61 }, { 216, 106,  28 }, { 255, 225,  38 },
  { 158, 225,  47 }, { 103, 181,  15 }, {  30,  30,  48 }, { 220, 255, 107 }, { 128, 255, 189 },
  { 154, 153, 255 }, { 142, 102, 255 }, {  64,  64,  64 }, { 117, 117, 117 }, { 224, 255, 255 },
  { 160,   0,   0 }, {  53,   0,   0 }, {  26, 208,   0 }, {   7,  66,   0 }, { 185, 176,   0 },
  {  63,  49,   0 }, { 179,  95,   0 }, {  75,  21,   2 },
};
// clang-format on
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

// clang-format off
enum class Push::Led : uint8_t
{
  TapTempo      = 3,
  Metronome     = 9,
  TouchStripTap = 12,
  Btn1Row1      = 20, // [RG]
  Btn2Row1      = 21, // [RG]
  Btn3Row1      = 22, // [RG]
  Btn4Row1      = 23, // [RG]
  Btn5Row1      = 24, // [RG]
  Btn6Row1      = 25, // [RG]
  Btn7Row1      = 26, // [RG]
  Btn8Row1      = 27, // [RG]
  Master        = 28,
  Stop          = 29,
  Grid1_4       = 36, // [RG]
  Grid1_4T      = 37, // [RG]
  Grid1_8       = 38, // [RG]
  Grid1_8T      = 39, // [RG]
  Grid1_16      = 40, // [RG]
  Grid1_16T     = 41, // [RG]
  Grid1_32      = 42, // [RG]
  Grid1_32T     = 43, // [RG]
  NavigateLeft  = 44,
  NavigateRight = 45,
  NavigateUp    = 46,
  NavigateDown  = 47,
  Select        = 48,
  Shift         = 49,
  Note          = 50,
  Session       = 51,
  AddEffect     = 52,
  AddTrack      = 53,
  OctaveDown    = 54,
  OctaveUp      = 55,
  Repeat        = 56,
  Accent        = 57,
  Scales        = 58,
  User          = 59,
  Solo          = 61,
  Mute          = 60,
  In            = 62,
  Out           = 63,
  Play          = 85,
  Rec           = 86,
  New           = 87,
  Duplicate     = 88,
  Automation    = 89,
  FixedLength   = 90,
  Btn1Row2      = 102, // [RGB]
  Btn2Row2      = 103, // [RGB]
  Btn3Row2      = 104, // [RGB]
  Btn4Row2      = 105, // [RGB]
  Btn5Row2      = 106, // [RGB]
  Btn6Row2      = 107, // [RGB]
  Btn7Row2      = 108, // [RGB]
  Btn8Row2      = 109, // [RGB]
  Device        = 110,
  Browse        = 111,
  Track         = 112,
  Clip          = 113,
  Volume        = 114,
  PanSend       = 115,
  Quantize      = 116,
  Double        = 117,
  Delete        = 118,
  Undo          = 119,

  Pad1,   Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
  Pad9,  Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16,
  Pad17, Pad18, Pad19, Pad20, Pad21, Pad22, Pad23, Pad24,
  Pad25, Pad26, Pad27, Pad28, Pad29, Pad30, Pad31, Pad32,
  Pad33, Pad34, Pad35, Pad36, Pad37, Pad38, Pad39, Pad40,
  Pad41, Pad42, Pad43, Pad44, Pad45, Pad46, Pad47, Pad48,
  Pad49, Pad50, Pad51, Pad52, Pad53, Pad54, Pad55, Pad56,
  Pad57, Pad58, Pad59, Pad60, Pad61, Pad62, Pad63, Pad64,

  Unknown,
};
// clang-format on

//--------------------------------------------------------------------------------------------------

enum class Push::Button : uint8_t
{
  TapTempo = 3,
  Metronome = 9,
  TouchStripTap = 12,
  Btn1Row1 = 20,
  Btn2Row1 = 21,
  Btn3Row1 = 22,
  Btn4Row1 = 23,
  Btn5Row1 = 24,
  Btn6Row1 = 25,
  Btn7Row1 = 26,
  Btn8Row1 = 27,
  Master = 28,
  Stop = 29,
  Grid1_4 = 36,
  Grid1_4T = 37,
  Grid1_8 = 38,
  Grid1_8T = 39,
  Grid1_16 = 40,
  Grid1_16T = 41,
  Grid1_32 = 42,
  Grid1_32T = 43,
  NavigateLeft = 44,
  NavigateRight = 45,
  NavigateUp = 46,
  NavigateDown = 47,
  Select = 48,
  Shift = 49,
  Note = 50,
  Session = 51,
  AddEffect = 52,
  AddTrack = 53,
  OctaveDown = 54,
  OctaveUp = 55,
  Repeat = 56,
  Accent = 57,
  Scales = 58,
  User = 59,
  Solo = 61,
  Mute = 60,
  In = 62,
  Out = 63,
  Play = 85,
  Rec = 86,
  New = 87,
  Duplicate = 88,
  Automation = 89,
  FixedLength = 90,
  Btn1Row2 = 102,
  Btn2Row2 = 103,
  Btn3Row2 = 104,
  Btn4Row2 = 105,
  Btn5Row2 = 106,
  Btn6Row2 = 107,
  Btn7Row2 = 108,
  Btn8Row2 = 109,
  Device = 110,
  Browse = 111,
  Track = 112,
  Clip = 113,
  Volume = 114,
  PanSend = 115,
  Quantize = 116,
  Double = 117,
  Delete = 118,
  Undo = 119,

  TouchEncoder1 = 128,
  TouchEncoder2,
  TouchEncoder3,
  TouchEncoder4,
  TouchEncoder5,
  TouchEncoder6,
  TouchEncoder7,
  TouchEncoder8,
  TouchEncoder9,
  TouchEncoderMain,
  TouchEncoderMain2,

  None,
};

//--------------------------------------------------------------------------------------------------

Push::Push() : m_isDirtyLeds(false)
{
  for (int i = 0; i < kPush_ledsDataSize; i++)
  {
    m_leds[i] = 0;
    m_ledsPrev[i] = 0;
  }
}

//--------------------------------------------------------------------------------------------------

void Push::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push::sendMidiMsg(tRawData midiMsg_)
{
  //!\todo Use Push virtual midi port
}

//--------------------------------------------------------------------------------------------------

TextDisplay* Push::textDisplay(size_t displayIndex_)
{
  static TextDisplayDummy s_dummyTextDisplay;
  if (displayIndex_ > kPush_nDisplays)
  {
    return &s_dummyTextDisplay;
  }

  return &m_displays[displayIndex_];
}

//--------------------------------------------------------------------------------------------------

bool Push::tick()
{
  bool success = true;

  if (m_displays[0].dirty() || m_displays[1].dirty() || m_displays[2].dirty()
      || m_displays[3].dirty())
  {
    success = false;
    success = sendDisplayData();
  }

  if (m_isDirtyLeds)
  {
    success |= sendLeds();
  }

  return success;
}

//--------------------------------------------------------------------------------------------------

void Push::init()
{
  // Display
  initDisplay();

  // Leds
  m_isDirtyLeds = true;
  std::fill(std::begin(m_leds), std::end(m_leds), 0);

  readFromDeviceHandleAsync(0, [this](Transfer transfer_) { process(transfer_.data()); });
}

//--------------------------------------------------------------------------------------------------

void Push::initDisplay() const
{
  //!\todo set backlight?
  return;
}

//--------------------------------------------------------------------------------------------------

bool Push::sendDisplayData()
{
  bool result = true;
  tRawData sysexHeader{kPush_manufacturerId, 0x7F, 0x15, 0x18, 0x00, 0x45, 0x00};

  for (unsigned row = 0; row < m_displays[0].height(); row++)
  {
    sysexHeader[3] = 0x18 + row;
    unsigned nCharsPerRow = m_displays[0].width();
    tRawData data(m_displays[0].width() * kPush_nDisplays);
    for (uint8_t i = 0; i < kPush_nDisplays; i++)
    {
      std::copy_n(
        m_displays[i].displayData() + (row * nCharsPerRow), nCharsPerRow, &data[i * nCharsPerRow]);
    }
    result = sendSysex({sysexHeader, data});
  }
  for (uint8_t i = 0; i < kPush_nDisplays; i++)
  {
    m_displays[i].resetDirtyFlags();
  }
  return result;
}

//--------------------------------------------------------------------------------------------------

bool Push::sendLeds()
{
  static const unsigned firstPadLed = static_cast<unsigned>(Led::Pad1);
  // if (m_isDirtyLeds)
  {
    for (size_t i = 0; i < m_leds.size(); i++)
    {
      if (m_ledsPrev[i] != m_leds[i])
      {
        m_ledsPrev[i] = m_leds[i];
        if (i < firstPadLed)
        {
          uint8_t led = static_cast<uint8_t>(i);
          if (!writeToDeviceHandle(Transfer({0xB0, led, m_leds[i]}), kPush_epOut))
          {
            return false;
          }
        }
        else
        {
          uint8_t led = static_cast<uint8_t>(i - firstPadLed + 36);
          if (!writeToDeviceHandle(Transfer({0x90, led, m_leds[i]}), kPush_epOut))
          {
            return false;
          }
        }
      }
    }
    //   m_isDirtyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

void Push::setLedImpl(Led led_, const Color& color_)
{
  uint8_t ledIndex = static_cast<uint8_t>(led_);

  if (Led::Unknown == led_)
  {
    return;
  }

  if (isRGBLed(led_))
  {
    uint8_t currentValue = m_leds[ledIndex];

    m_leds[ledIndex] = getColorIndex(color_);
    m_isDirtyLeds = m_isDirtyLeds || currentValue != m_leds[ledIndex];
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

bool Push::isRGBLed(Led led_) const noexcept
{
  if (Led::Undo < led_ || Led::Btn1Row2 == led_ || Led::Btn2Row2 == led_ || Led::Btn3Row2 == led_
      || Led::Btn4Row2 == led_
      || Led::Btn5Row2 == led_
      || Led::Btn6Row2 == led_
      || Led::Btn7Row2 == led_
      || Led::Btn8Row2 == led_)
  {
    //\todo handle RG leds
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

Push::Led Push::led(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(TapTempo);
    M_LED_CASE(Metronome);
    M_LED_CASE(TouchStripTap);
    M_LED_CASE(Btn1Row1);
    M_LED_CASE(Btn2Row1);
    M_LED_CASE(Btn3Row1);
    M_LED_CASE(Btn4Row1);
    M_LED_CASE(Btn5Row1);
    M_LED_CASE(Btn6Row1);
    M_LED_CASE(Btn7Row1);
    M_LED_CASE(Btn8Row1);
    M_LED_CASE(Master);
    M_LED_CASE(Stop);
    M_LED_CASE(Grid1_4);
    M_LED_CASE(Grid1_4T);
    M_LED_CASE(Grid1_8);
    M_LED_CASE(Grid1_8T);
    M_LED_CASE(Grid1_16);
    M_LED_CASE(Grid1_16T);
    M_LED_CASE(Grid1_32);
    M_LED_CASE(Grid1_32T);
    M_LED_CASE(NavigateLeft);
    M_LED_CASE(NavigateRight);
    M_LED_CASE(NavigateUp);
    M_LED_CASE(NavigateDown);
    M_LED_CASE(Select);
    M_LED_CASE(Shift);
    M_LED_CASE(Note);
    M_LED_CASE(Session);
    M_LED_CASE(AddEffect);
    M_LED_CASE(AddTrack);
    M_LED_CASE(OctaveDown);
    M_LED_CASE(OctaveUp);
    M_LED_CASE(Repeat);
    M_LED_CASE(Accent);
    M_LED_CASE(Scales);
    M_LED_CASE(User);
    M_LED_CASE(Solo);
    M_LED_CASE(Mute);
    M_LED_CASE(In);
    M_LED_CASE(Out);
    M_LED_CASE(Play);
    M_LED_CASE(Rec);
    M_LED_CASE(New);
    M_LED_CASE(Duplicate);
    M_LED_CASE(Automation);
    M_LED_CASE(FixedLength);
    M_LED_CASE(Btn1Row2);
    M_LED_CASE(Btn2Row2);
    M_LED_CASE(Btn3Row2);
    M_LED_CASE(Btn4Row2);
    M_LED_CASE(Btn5Row2);
    M_LED_CASE(Btn6Row2);
    M_LED_CASE(Btn7Row2);
    M_LED_CASE(Btn8Row2);
    M_LED_CASE(Device);
    M_LED_CASE(Browse);
    M_LED_CASE(Track);
    M_LED_CASE(Clip);
    M_LED_CASE(Volume);
    M_LED_CASE(PanSend);
    M_LED_CASE(Quantize);
    M_LED_CASE(Double);
    M_LED_CASE(Delete);
    M_LED_CASE(Undo);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

Push::Led Push::led(unsigned index_) const noexcept
{
  unsigned index = static_cast<unsigned>(Led::Pad1) + index_;
  if (index_ < 64)
  {
    return static_cast<Led>(index);
  }

  return Led::Unknown;
}

//--------------------------------------------------------------------------------------------------

Device::Button Push::deviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(TapTempo);
    M_BTN_CASE(Metronome);
    M_BTN_CASE(TouchStripTap);
    M_BTN_CASE(Btn1Row1);
    M_BTN_CASE(Btn2Row1);
    M_BTN_CASE(Btn3Row1);
    M_BTN_CASE(Btn4Row1);
    M_BTN_CASE(Btn5Row1);
    M_BTN_CASE(Btn6Row1);
    M_BTN_CASE(Btn7Row1);
    M_BTN_CASE(Btn8Row1);
    M_BTN_CASE(Master);
    M_BTN_CASE(Stop);
    M_BTN_CASE(Grid1_4);
    M_BTN_CASE(Grid1_4T);
    M_BTN_CASE(Grid1_8);
    M_BTN_CASE(Grid1_8T);
    M_BTN_CASE(Grid1_16);
    M_BTN_CASE(Grid1_16T);
    M_BTN_CASE(Grid1_32);
    M_BTN_CASE(Grid1_32T);
    M_BTN_CASE(NavigateLeft);
    M_BTN_CASE(NavigateRight);
    M_BTN_CASE(NavigateUp);
    M_BTN_CASE(NavigateDown);
    M_BTN_CASE(Select);
    M_BTN_CASE(Shift);
    M_BTN_CASE(Note);
    M_BTN_CASE(Session);
    M_BTN_CASE(AddEffect);
    M_BTN_CASE(AddTrack);
    M_BTN_CASE(OctaveDown);
    M_BTN_CASE(OctaveUp);
    M_BTN_CASE(Repeat);
    M_BTN_CASE(Accent);
    M_BTN_CASE(Scales);
    M_BTN_CASE(User);
    M_BTN_CASE(Solo);
    M_BTN_CASE(Mute);
    M_BTN_CASE(In);
    M_BTN_CASE(Out);
    M_BTN_CASE(Play);
    M_BTN_CASE(Rec);
    M_BTN_CASE(New);
    M_BTN_CASE(Duplicate);
    M_BTN_CASE(Automation);
    M_BTN_CASE(FixedLength);
    M_BTN_CASE(Btn1Row2);
    M_BTN_CASE(Btn2Row2);
    M_BTN_CASE(Btn3Row2);
    M_BTN_CASE(Btn4Row2);
    M_BTN_CASE(Btn5Row2);
    M_BTN_CASE(Btn6Row2);
    M_BTN_CASE(Btn7Row2);
    M_BTN_CASE(Btn8Row2);
    M_BTN_CASE(Device);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Track);
    M_BTN_CASE(Clip);
    M_BTN_CASE(Volume);
    M_BTN_CASE(PanSend);
    M_BTN_CASE(Quantize);
    M_BTN_CASE(Double);
    M_BTN_CASE(Delete);
    M_BTN_CASE(Undo);
    M_BTN_CASE(TouchEncoder1);
    M_BTN_CASE(TouchEncoder2);
    M_BTN_CASE(TouchEncoder3);
    M_BTN_CASE(TouchEncoder4);
    M_BTN_CASE(TouchEncoder5);
    M_BTN_CASE(TouchEncoder6);
    M_BTN_CASE(TouchEncoder7);
    M_BTN_CASE(TouchEncoder8);
    M_BTN_CASE(TouchEncoder9);
    M_BTN_CASE(TouchEncoderMain);
    M_BTN_CASE(TouchEncoderMain2);
    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

uint8_t Push::getColorIndex(const Color& color_)
{
  auto it = m_colorsCache.find(color_);
  if (it != m_colorsCache.end())
  {
    return it->second;
  }

  uint8_t colorIndex = 0;
  double minDistance = std::numeric_limits<double>::max();
  for (uint8_t i = 0; i < kPush_colors.size(); i++)
  {
    double currentDistance = color_.distance(kPush_colors[i]);
    if (currentDistance < minDistance)
    {
      colorIndex = i;
      minDistance = currentDistance;
    }
    if (minDistance == 0)
    {
      break;
    }
  }
  m_colorsCache.emplace(std::move(color_), colorIndex);
  return colorIndex;
}

//--------------------------------------------------------------------------------------------------

void Push::onNoteOff(NoteOff msg_)
{
  processNote(msg_.data()[1], 0);
}

//--------------------------------------------------------------------------------------------------

void Push::onNoteOn(NoteOn msg_)
{
  processNote(msg_.data()[1], msg_.data()[2]);
}

//--------------------------------------------------------------------------------------------------

void Push::onPolyPressure(PolyPressure msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push::onControlChange(ControlChange msg_)
{
  uint8_t cc = msg_.getControl();
  uint8_t value = msg_.getValue();

  if (cc == static_cast<unsigned>(Device::Button::Shift))
  {
    m_shiftPressed = value > 0;
    return;
  }
  
#define M_ENC_CASE(cc, index) \
  case cc:    \
    return encoderChanged(index, value < 64, m_shiftPressed)

  switch (cc)
  {
    M_ENC_CASE(71,1);
    M_ENC_CASE(72,2);
    M_ENC_CASE(73,3);
    M_ENC_CASE(74,4);
    M_ENC_CASE(75,5);
    M_ENC_CASE(76,6);
    M_ENC_CASE(77,7);
    M_ENC_CASE(78,8);
    M_ENC_CASE(79,9);
    M_ENC_CASE(14,0);
    M_ENC_CASE(15,10);
  }
  
  Device::Button changedButton = deviceButton(static_cast<Button>(cc));
  buttonChanged(changedButton, value > 0, m_shiftPressed);
  
#undef M_ENC_CASE
}

//--------------------------------------------------------------------------------------------------

void Push::onProgramChange(ProgramChange msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push::onChannelPressure(ChannelPressure msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push::onPitchBend(PitchBend msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push::onSysEx(SysEx msg_)
{
}
//--------------------------------------------------------------------------------------------------

void Push::onUSysExRT(sl::midi::USysExRT msg_)
{
}
//--------------------------------------------------------------------------------------------------

void Push::onUSysExNonRT(sl::midi::USysExNonRT msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push::processNote(uint8_t note_, uint8_t velocity_)
{
  if (note_ <= 10)
  {
    // Touch encoders
    uint8_t offset = static_cast<uint8_t>(Button::TouchEncoder1);
    Device::Button btn = deviceButton(static_cast<Button>(note_ + offset));
    buttonChanged(btn, (velocity_ > 0), m_shiftPressed);
  }
  else if (note_ == 12)
  {
    // Pitch bend
  }
  else if (note_ >= 36 && note_ <= 99)
  {
    // Pads
    keyChanged(note_ - 36, velocity_ / 127.0, m_shiftPressed);
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
