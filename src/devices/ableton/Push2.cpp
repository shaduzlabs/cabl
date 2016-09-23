/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ableton/Push2.h"

#include <algorithm>
#include <limits>
#include <thread>

#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"

#include "cabl/gfx/TextDisplay.h"
#include "gfx/displays/NullCanvas.h"

#include <cmath>

//!\todo delete debug includes
#include <iomanip>
#include <iostream>

//--------------------------------------------------------------------------------------------------

namespace
{
const std::string kPush2_midiPortName = "Ableton Push 2 Live Port";
const uint8_t kPush_epOut = 0x01;

// clang-format off
const std::vector<sl::cabl::Color> kPush_colors{
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
//| R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
  {   0,   0,   0 }, { 255,  36,  36 }, { 242,  58,  12 }, { 255, 153,   0 }, { 166, 137,  86 },
  { 237, 249,  90 }, { 193, 157,   8 }, { 255, 255,   0 }, {  86, 191,  19 }, {  44, 132,   3 },
  {  36, 107,  36 }, {  25, 255,  48 }, {  21, 149, 115 }, {  23, 107,  80 }, {   0, 255, 255 },
  {   0, 116, 252 }, {  39,  79, 204 }, {   0,  68, 140 }, { 100,  74, 217 }, {  77,  63, 160 },
  { 135,   0, 255 }, { 230,  87, 227 }, { 102,   0, 153 }, { 255,   0, 153 }, { 161,  76,  95 },
  { 255,  77, 196 }, { 235, 139, 225 }, { 166,  52,  33 }, { 153,  86,  40 }, { 135, 103,   0 },
  { 144, 130,  31 }, {  74, 135,   0 }, {   0, 127,  18 }, {  24,  83, 178 }, {  98,  75, 173 },
  { 115,  58, 103 }, { 248, 188, 175 }, { 255, 155, 118 }, { 255, 191,  95 }, { 217, 175, 113 },
  { 255, 244, 128 }, { 191, 186, 105 }, { 188, 204, 136 }, { 174, 255, 153 }, { 124, 221, 159 },
  { 137, 180, 125 }, { 128, 243, 255 }, { 122, 206, 252 }, { 104, 161, 211 }, { 133, 143, 194 },
  { 187, 170, 242 }, { 205, 187, 228 }, { 239, 139, 176 }, { 133, 157, 140 }, { 107, 117, 110 },
  { 132, 144, 155 }, { 106, 112, 117 }, { 136, 133, 157 }, { 108, 106, 117 }, { 157, 133, 156 },
  { 116, 106, 116 }, { 156, 157, 133 }, { 116, 117, 106 }, { 157, 132, 132 }, { 117, 106, 106 },
  {  77,  11,  11 }, {  26,   4,   4 }, {  77,  18,   4 }, {  26,   6,   1 }, {  77,  46,   0 },
  {  26,  15,   0 }, {  64,  52,  33 }, {  26,  21,  13 }, {  77,  73,  31 }, {  26,  24,  10 },
  {  64,  51,   2 }, {  26,  21,   1 }, {  77,  77,   0 }, {  26,  26,   0 }, {  28,  64,   7 },
  {  11,  26,   3 }, {  17,  51,   1 }, {   4,  13,   0 }, {  17,  51,  17 }, {   4,  13,   4 },
  {  10,  77,  10 }, {   3,  26,   5 }, {   7,  51,  39 }, {   2,  13,  10 }, {  16,  77,  57 },
  {   3,  13,  10 }, {   0,  77,  77 }, {   0,  26,  26 }, {   0,  35,  77 }, {   0,  12,  26 },
  {  12,  25,  64 }, {   2,   5,  13 }, {   0,  37,  77 }, {   0,  12,  26 }, {  35,  26,  77 },
  {  12,   9,  26 }, {  37,  30,  77 }, {  12,  10,  26 }, {  54,   0, 102 }, {  13,   0,  26 },
  {  77,  29,  76 }, {  26,  10,  25 }, {  51,   0,  77 }, {  17,   0,  26 }, {  77,   0,  46 },
  {  26,   0,  15 }, {  77,  36,  45 }, {  26,  12,  15 }, {  77,  23,  59 }, {  26,   8,  20 },
  {  77,  45,  73 }, {  26,  15,  24 }, {   0,   0,   0 }, {  89,  89,  89 }, {  26,  26,  26 },
  { 255, 255, 255 }, {  89,  89,  89 }, { 255, 255, 255 }, {  89,  89,  89 }, {  26,  26,  26 },
  {   0,   0, 255 }, {   0, 255,   0 }, {  25, 128,   0 },
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
enum class Push2::Led : uint8_t
{
  TapTempo      = 3,
  Metronome     = 9,
  TouchStripTap = 12,
  Btn1Row1      = 20, // [RGB]
  Btn2Row1      = 21, // [RGB]
  Btn3Row1      = 22, // [RGB]
  Btn4Row1      = 23, // [RGB]
  Btn5Row1      = 24, // [RGB]
  Btn6Row1      = 25, // [RGB]
  Btn7Row1      = 26, // [RGB]
  Btn8Row1      = 27, // [RGB]
  Master        = 28,
  Stop          = 29,
  Setup         = 30,
  Layout        = 31,
  Convert       = 35,
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

enum class Push2::Button : uint8_t
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
  Setup = 30,
  Layout = 31,
  Convert = 35,
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

Push2::Push2() : m_pMidiOut(new RtMidiOut), m_pMidiIn(new RtMidiIn)
{
  std::string portName;
  unsigned nPorts = m_pMidiOut->getPortCount();
  for (unsigned int i = 0; i < nPorts; i++)
  {
    try
    {
      portName = m_pMidiOut->getPortName(i);
      if (portName.find(kPush2_midiPortName) != std::string::npos && portName.back() == '1')
      {
        m_pMidiOut->openPort(i);
      }
    }
    catch (RtMidiError& error)
    {
      std::string strError(error.getMessage());
      M_LOG("[Push2] RtMidiError: " << strError);
    }
  }
  if (!m_pMidiOut->isPortOpen())
  {
    m_pMidiOut.reset(nullptr);
  }

  portName.clear();
  nPorts = m_pMidiIn->getPortCount();
  for (unsigned int i = 0; i < nPorts; i++)
  {
    try
    {
      portName = m_pMidiIn->getPortName(i);
      if (portName.find(kPush2_midiPortName) != std::string::npos && portName.back() == '1')
      {
        m_pMidiIn->openPort(i);
      }
    }
    catch (RtMidiError& error)
    {
      std::string strError(error.getMessage());
      M_LOG("[Push2] RtMidiError: " << strError);
    }
  }
  if (!m_pMidiIn->isPortOpen())
  {
    m_pMidiIn.reset(nullptr);
  }
  else
  {
    m_pMidiIn->setCallback(&Push2::midiInCallback, this);
  }
}

//--------------------------------------------------------------------------------------------------

void Push2::setButtonLed(Device::Button btn_, const Color& color_)
{
  setLedImpl(led(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push2::setKeyLed(unsigned index_, const Color& color_)
{
  setLedImpl(led(index_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push2::sendMidiMsg(tRawData midiMsg_)
{
  //!\todo Use Push virtual midi port
}

//--------------------------------------------------------------------------------------------------

bool Push2::tick()
{
  bool success = false;

  if (m_isDirtyLeds)
  {
    success |= sendLeds();
  }

  return success;
}

//--------------------------------------------------------------------------------------------------

void Push2::init()
{
  // Leds
  std::fill(std::begin(m_leds), std::end(m_leds), 0);
  m_isDirtyLeds = true;
}

//--------------------------------------------------------------------------------------------------

bool Push2::sendLeds()
{
  //  if (m_isDirtyLeds)
  {
    if (!writeToDeviceHandle(Transfer({0x80}, &m_leds[0], 78), kPush_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

void Push2::setLedImpl(Led led_, const Color& color_)
{
  unsigned ledIndex = static_cast<uint8_t>(led_);

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

bool Push2::isRGBLed(Led led_) const noexcept
{
  if (Led::Undo < led_ || (Led::Btn1Row1 <= led_ && Led::Btn8Row1 >= led_)
      || (Led::Btn1Row2 <= led_ && Led::Btn8Row2 >= led_))
  {
    //\todo handle RG leds
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

Push2::Led Push2::led(Device::Button btn_) const noexcept
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
    M_LED_CASE(Setup);
    M_LED_CASE(Layout);
    M_LED_CASE(Convert);
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

Push2::Led Push2::led(unsigned index_) const noexcept
{
  unsigned index = static_cast<unsigned>(Led::Pad1) + index_;
  if (index_ < 64)
  {
    return static_cast<Led>(index);
  }

  return Led::Unknown;
}

//--------------------------------------------------------------------------------------------------

Device::Button Push2::deviceButton(Button btn_) const noexcept
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
    M_BTN_CASE(Setup);
    M_BTN_CASE(Layout);
    M_BTN_CASE(Convert);
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

uint8_t Push2::getColorIndex(const Color& color_)
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

void Push2::onNoteOff(NoteOff msg_)
{
  processNote(msg_.data()[1], 0);
}

//--------------------------------------------------------------------------------------------------

void Push2::onNoteOn(NoteOn msg_)
{
  processNote(msg_.data()[1], msg_.data()[2]);
}

//--------------------------------------------------------------------------------------------------

void Push2::onPolyPressure(PolyPressure msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push2::onControlChange(ControlChange msg_)
{
  uint8_t cc = msg_.getControl();
  uint8_t value = msg_.getValue();

  if (cc == static_cast<unsigned>(Device::Button::Shift))
  {
    m_shiftPressed = value > 0;
    return;
  }

#define M_ENC_CASE(cc, index) \
  case cc:                    \
    return encoderChanged(index, value < 64, m_shiftPressed)

  switch (cc)
  {
    M_ENC_CASE(71, 1);
    M_ENC_CASE(72, 2);
    M_ENC_CASE(73, 3);
    M_ENC_CASE(74, 4);
    M_ENC_CASE(75, 5);
    M_ENC_CASE(76, 6);
    M_ENC_CASE(77, 7);
    M_ENC_CASE(78, 8);
    M_ENC_CASE(79, 9);
    M_ENC_CASE(14, 0);
    M_ENC_CASE(15, 10);
  }

  Device::Button changedButton = deviceButton(static_cast<Button>(cc));
  buttonChanged(changedButton, value > 0, m_shiftPressed);

#undef M_ENC_CASE
}

//--------------------------------------------------------------------------------------------------

void Push2::onProgramChange(ProgramChange msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push2::onChannelPressure(ChannelPressure msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push2::onPitchBend(PitchBend msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push2::onSysEx(SysEx msg_)
{
}
//--------------------------------------------------------------------------------------------------

void Push2::onUSysExRT(sl::midi::USysExRT msg_)
{
}
//--------------------------------------------------------------------------------------------------

void Push2::onUSysExNonRT(sl::midi::USysExNonRT msg_)
{
}

//--------------------------------------------------------------------------------------------------

void Push2::processNote(uint8_t note_, uint8_t velocity_)
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

void Push2::midiInCallback(
  double timeStamp_, std::vector<unsigned char>* pMessage_, void* userData_)
{
  Push2* pSelf = static_cast<Push2*>(userData_);
  if ((pMessage_->at(0) & 0xf0) == 0x90)
  {
    pSelf->processNote(pMessage_->at(1), pMessage_->at(2));
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
