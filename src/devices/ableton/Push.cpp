/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ableton/Push.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions.h"

#include <thread>

#include "gfx/LCDDisplay.h"
#include "gfx/displays/GDisplayDummy.h"

#include <cmath>

//!\todo delete debug includes
#include <iostream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------

namespace
{
static const uint8_t kPush_epOut = 0x01;
static const uint8_t kPush_epInput = 0x81;
static const uint8_t kPush_manufacturerId = 0x47; // Akai manufacturer Id

static const std::vector<sl::util::RGBColor> kPush_colors =  {{  0,  0,  0}, { 30, 30, 30},
  {127,127,127}, {255,255,255}, {255, 76, 76}, {255,  0,  0}, { 89,  0,  0}, { 25,  0,  0},
  {255,189,108}, {255, 84,  0}, { 89, 29,  0}, { 39, 27,  0}, {255,255, 76}, {255,255,  0},
  { 89, 89,  0}, { 25, 25,  0}, {136,255, 76}, { 84,255,  0}, { 29, 89,  0}, { 20, 43,  0},
  { 76,255, 76}, {  0,255,  0}, {  0, 89,  0}, {  0, 25,  0}, { 76,255, 94}, {  0,255, 25},
  {  0, 89, 13}, {  0, 25,  2}, { 76,255,136}, {  0,255, 85}, {  0, 89, 29}, {  0, 31, 18},
  { 76,255,183}, {  0,255,153}, {  0, 89, 53}, {  0, 25, 18}, { 76,195,255}, {  0,169,255},
  {  0, 65, 82}, {  0, 16, 25}, { 76,136,255}, {  0, 85,255}, {  0, 29, 89}, {  0,  8, 25},
  { 76, 76,255}, {  0,  0,255}, {  0,  0, 89}, {  0,  0, 25}, {135, 76,255}, { 84,  0,255},
  { 25,  0,100}, { 15,  0, 48}, {255, 76,255}, {255,  0,255}, { 89,  0, 89}, { 25,  0, 25},
  {255, 76,135}, {255,  0, 84}, { 89,  0, 29}, { 34,  0, 19}, {255, 21,  0}, {153, 53,  0},
  {121, 81,  0}, { 67,100,  0}, {  3, 57,  0}, {  0, 87, 53}, {  0, 84,127}, {  0,  0,255},
  {  0, 69, 79}, { 37,  0,204}, {127,127,127}, { 32, 32, 32}, {255,  0,  0}, {189,255, 45},
  {175,237,  6}, {100,255,  9}, { 16,139,  0}, {  0,255,135}, {  0,169,255}, {  0, 42,255},
  { 63,  0,255}, {122,  0,255}, {178, 26,125}, { 64, 33,  0}, {255, 74,  0}, {136,225,  6},
  {114,255, 21}, {  0,255,  0}, { 59,255, 38}, { 89,255,113}, { 56,255,204}, { 91,138,255},
  { 49, 81,198}, {135,127,233}, {211, 29,255}, {255,  0, 93}, {255,127,  0}, {185,176,  0},
  {144,255,  0}, {131, 93,  7}, { 57, 43,  0}, { 20, 76, 16}, { 13, 80, 56}, { 21, 21, 42},
  { 22, 32, 90}, {105, 60, 28}, {168,  0, 10}, {222, 81, 61}, {216,106, 28}, {255,225, 38},
  {158,225, 47}, {103,181, 15}, { 30, 30, 48}, {220,255,107}, {128,255,189}, {154,153,255},
  {142,102,255}, { 64, 64, 64}, {117,117,117}, {224,255,255}, {160,  0,  0}, { 53,  0,  0},
  { 26,208,  0}, {  7, 66,  0}, {185,176,  0}, { 63, 49,  0}, {179, 95,  0}, { 75, 21,  2}};


}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

enum class Push::Led : uint8_t
{
  TapTempo      =   3,
  Metronome     =   9,
  Btn1Row1      =  20, // [RG]
  Btn2Row1      =  21, // [RG]
  Btn3Row1      =  22, // [RG]
  Btn4Row1      =  23, // [RG]
  Btn5Row1      =  24, // [RG]
  Btn6Row1      =  25, // [RG]
  Btn7Row1      =  26, // [RG]
  Btn8Row1      =  27, // [RG]
  Master        =  28,
  Stop          =  29,
  Grid1_4       =  36, // [RG]
  Grid1_4T      =  37, // [RG]
  Grid1_8       =  38, // [RG]
  Grid1_8T      =  39, // [RG]
  Grid1_16      =  40, // [RG]
  Grid1_16T     =  41, // [RG]
  Grid1_32      =  42, // [RG]
  Grid1_32T     =  43, // [RG]
  NavigateLeft  =  44,
  NavigateRight =  45,
  NavigateUp    =  46,
  NavigateDown  =  47,
  Select        =  48,
  Shift         =  49,
  Note          =  50,
  Session       =  51,
  AddEffect     =  52,
  AddTrack      =  53,
  OctaveDown    =  54,
  OctaveUp      =  55,
  Repeat        =  56,
  Accent        =  57,
  Scales        =  58,
  User          =  59,
  Solo          =  61,
  Mute          =  60,
  In            =  62,
  Out           =  63,
  Play          =  85,
  Rec           =  86,
  New           =  87,
  Duplicate     =  88,
  Automation    =  89,
  FixedLength   =  90,
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
  
  Pad1,  Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
  Pad9,  Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16, 
  Pad17, Pad18, Pad19, Pad20, Pad21, Pad22, Pad23, Pad24, 
  Pad25, Pad26, Pad27, Pad28, Pad29, Pad30, Pad31, Pad32, 
  Pad33, Pad34, Pad35, Pad36, Pad37, Pad38, Pad39, Pad40, 
  Pad41, Pad42, Pad43, Pad44, Pad45, Pad46, Pad47, Pad48, 
  Pad49, Pad50, Pad51, Pad52, Pad53, Pad54, Pad55, Pad56, 
  Pad57, Pad58, Pad59, Pad60, Pad61, Pad62, Pad63, Pad64,

  Unknown,
};

//--------------------------------------------------------------------------------------------------


enum class Push::Button : uint8_t
{
  TapTempo      =   3,
  Metronome     =   9,
  Btn1Row1      =  20,
  Btn2Row1      =  21,
  Btn3Row1      =  22,
  Btn4Row1      =  23,
  Btn5Row1      =  24,
  Btn6Row1      =  25,
  Btn7Row1      =  26,
  Btn8Row1      =  27,
  Master        =  28,
  Stop          =  29,
  Grid1_4       =  36,
  Grid1_4T      =  37,
  Grid1_8       =  38,
  Grid1_8T      =  39,
  Grid1_16      =  40,
  Grid1_16T     =  41,
  Grid1_32      =  42,
  Grid1_32T     =  43,
  NavigateLeft  =  44,
  NavigateRight =  45,
  NavigateUp    =  46,
  NavigateDown  =  47,
  Select        =  48,
  Shift         =  49,
  Note          =  50,
  Session       =  51,
  AddEffect     =  52,
  AddTrack      =  53,
  OctaveDown    =  54,
  OctaveUp      =  55,
  Repeat        =  56,
  Accent        =  57,
  Scales        =  58,
  User          =  59,
  Solo          =  61,
  Mute          =  60,
  In            =  62,
  Out           =  63,
  Play          =  85,
  Rec           =  86,
  New           =  87,
  Duplicate     =  88,
  Automation    =  89,
  FixedLength   =  90,
  Btn1Row2      = 102,
  Btn2Row2      = 103,
  Btn3Row2      = 104,
  Btn4Row2      = 105,
  Btn5Row2      = 106,
  Btn6Row2      = 107,
  Btn7Row2      = 108,
  Btn8Row2      = 109,
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

Push::Push(tPtr<DeviceHandle> pDeviceHandle_)
  : USBMidi(std::move(pDeviceHandle_))
  , m_isDirtyLeds(false)
{
 m_buttons.resize(kPush_buttonsDataSize);
 m_leds.resize(kPush_ledsDataSize);
}

//--------------------------------------------------------------------------------------------------

Push::~Push()
{

}

//--------------------------------------------------------------------------------------------------

void Push::setLed(Device::Button btn_, const util::LedColor& color_)
{
  setLedImpl(getLed(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push::setLed(Device::Pad pad_, const util::LedColor& color_)
{
  setLedImpl(getLed(pad_), color_);
}

//--------------------------------------------------------------------------------------------------

void Push::sendMidiMsg(tRawData midiMsg_)
{
 //!\todo Use Push virtual midi port
}

//--------------------------------------------------------------------------------------------------

GDisplay* Push::getGraphicDisplay(uint8_t displayIndex_)
{
  static GDisplayDummy s_dummyDisplay;
  return &s_dummyDisplay;
}

//--------------------------------------------------------------------------------------------------

LCDDisplay* Push::getLCDDisplay(uint8_t displayIndex_)
{
  static LCDDisplay s_dummyLCDDisplay(0, 0);
  if (displayIndex_ > kPush_nDisplays)
  {
    return &s_dummyLCDDisplay;
  }

  return &m_displays[displayIndex_];
}

//--------------------------------------------------------------------------------------------------

bool Push::tick()
{
  static int state = 0;
  bool success = false;

  //!\todo enable once display dirty flag is properly set
  if (state == 0 && (
       m_displays[0].isDirty() ||
       m_displays[1].isDirty() ||
       m_displays[2].isDirty() ||
       m_displays[3].isDirty()
      )
   )
  {
    success = sendDisplayData();
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

void Push::init()
{
  // Display
  initDisplay();

  // Leds
  m_isDirtyLeds = true;
}

//--------------------------------------------------------------------------------------------------

void Push::initDisplay() const
{
  //!\todo set backlight
  return;
}

//--------------------------------------------------------------------------------------------------

bool Push::sendDisplayData()
{
  bool result = true;
  tRawData sysexHeader{kPush_manufacturerId, 0x7F, 0x15, 0x18, 0x00, 0x45, 0x00 };
  
  for(uint8_t row = 0; row < m_displays[0].getNumberOfRows(); row++)
  {
    sysexHeader[3] = 0x18 + row;
    uint8_t nCharsPerRow = m_displays[0].getNumberOfCharsPerRow();
    tRawData data(m_displays[0].getNumberOfCharsPerRow() * kPush_nDisplays);
    for(uint8_t i = 0; i < kPush_nDisplays; i++)
    {
      std::copy_n( m_displays[i].getData().data() + (row*nCharsPerRow),
                   nCharsPerRow,
                   &data[i*nCharsPerRow]
      );
    }
    result = sendSysex({sysexHeader,data});    
  }
  
  return result;
}
//--------------------------------------------------------------------------------------------------

bool Push::sendLeds()
{
//  if (m_isDirtyLeds)
  {
    if(!getDeviceHandle()->write(Transfer({0x80}, &m_leds[0], 78), kPush_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool Push::read()
{
  Transfer input;
  for (uint8_t n = 0; n < 32; n++)
  {
    if (!getDeviceHandle()->read(input, kPush_epInput))
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

void Push::processButtons(const Transfer& input_)
{
  bool shiftPressed(isButtonPressed(input_, Button::Shift));
  Device::Button changedButton(Device::Button::Unknown);
  bool buttonPressed(false);

  for (int i = 0; i < kPush_buttonsDataSize - 1; i++) // Skip the last byte (encoder value)
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
      //    std::copy(&input_[1],&input_[kPush_buttonsDataSize],m_buttons.begin());
          buttonChanged(changedButton, buttonPressed, shiftPressed);
        }
      }
    }
  }

  // Now process the encoder data
  uint8_t currentEncoderValue = input_.getData()[kPush_buttonsDataSize];
  if (m_encoderValue != currentEncoderValue)
  {
    bool valueIncreased = ((m_encoderValue < currentEncoderValue) || 
      ((m_encoderValue == 0x0f) && (currentEncoderValue == 0x00)))
        && (!((m_encoderValue == 0x0) && (currentEncoderValue == 0x0f)));
      encoderChanged(Device::Encoder::Main, valueIncreased, shiftPressed);
    m_encoderValue = currentEncoderValue;
  }
}

//--------------------------------------------------------------------------------------------------

void Push::processPads(const Transfer& input_)
{
  //!\todo process pad data
  for (int i = 1; i <= kPush_padDataSize; i += 2)
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

//--------------------------------------------------------------------------------------------------

void Push::setLedImpl(Led led_, const util::LedColor& color_)
{
  uint8_t ledIndex = static_cast<uint8_t>(led_);

  if(Led::Unknown == led_)
  {
    return;
  }

  if (isRGBLed(led_))
  {
    uint8_t currentValue = m_leds[ledIndex];

    m_leds[ledIndex] =

    m_isDirtyLeds = m_isDirtyLeds || currentValue != m_leds[ledIndex];
  }
  else
  {
    uint8_t currentVal = m_leds[ledIndex];
    uint8_t newVal = color_.getMono();

    m_leds[ledIndex] = newVal;
    m_isDirtyLeds = m_isDirtyLeds || (currentVal != newVal);
  }
}

//--------------------------------------------------------------------------------------------------

bool Push::isRGBLed(Led led_) const noexcept
{
  if (Led::Undo       < led_ || Led::Btn1Row2  == led_ || Led::Btn2Row2  == led_ ||
      Led::Btn3Row2  == led_ || Led::Btn4Row2  == led_ || Led::Btn5Row2  == led_ ||
      Led::Btn6Row2  == led_ || Led::Btn7Row2  == led_ || Led::Btn8Row2  == led_  )
  {
    //\todo handle RG leds
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

Push::Led Push::getLed(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(TapTempo);
    M_LED_CASE(Metronome);
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

Push::Led Push::getLed(Device::Pad pad_) const noexcept
{
  uint8_t pad = static_cast<unsigned>(pad_);
  unsigned index = static_cast<unsigned>(Led::Pad1) + pad;
  if(pad < 64)
  {
    return static_cast<Led>(index);
  }
  
  return Led::Unknown;
}

//--------------------------------------------------------------------------------------------------

Device::Button Push::getDeviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(TapTempo);
    M_BTN_CASE(Metronome);
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

bool Push::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool Push::isButtonPressed(
  const Transfer& transfer_, 
  Button button_
) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

uint8_t Push::getColorIndex(const util::LedColor& color_)
{
  util::RGBColor ledColor(color_.getRGBColor());
  
  auto it = m_colorsCache.find(ledColor);
  if(it != m_colorsCache.end())
  {
    return it->second;
  }

  uint8_t colorIndex = 0;
  double minDistance = std::numeric_limits<double>::max();
  for(uint8_t i = 0; i < kPush_colors.size(); i++)
  {
    double currentDistance = ledColor.distance(kPush_colors[i]);
    if(currentDistance < minDistance)
    {
      colorIndex = i;
      minDistance = currentDistance;
    }
    if(minDistance == 0)
    {
      break;
    }
  }
  m_colorsCache.emplace(std::move(ledColor), colorIndex);
  return colorIndex;
}

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
