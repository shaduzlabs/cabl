/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <algorithm>

#include "devices/ni/KompleteKontrol.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions.h"
#include "gfx/LCDDisplay.h"

//--------------------------------------------------------------------------------------------------

namespace
{
static const std::string kKK_midiPortName = "KOMPLETE KONTROL S";
static const uint8_t kKK_ledsDataSize     = 25;

static const uint8_t kKK_epOut   = 0x02;
static const uint8_t kKK_epInput = 0x84;
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

enum class KompleteKontrol::Led : uint16_t
{
  Shift,
  Scale,
  Arp,
  Loop,
  Rwd,
  Ffw,
  Play,
  Rec,
  Stop,
  PageLeft,
  PageRight,
  Browse,
  PresetUp,
  Instance,
  PresetDown,
  Back,
  NavigateUp,
  Enter,
  NavigateLeft,
  NavigateDown,
  NavigateRight,
  OctaveDownWhite,
  OctaveDownRed,
  OctaveUpWhite,
  OctaveUpRed,

  Key1,
  Key1_R = Key1,
  Key1_G,
  Key1_B,
  Key2,
  Key2_R = Key2,
  Key2_G,
  Key2_B,
  Key3,
  Key3_R = Key3,
  Key3_G,
  Key3_B,
  Key4,
  Key4_R = Key4,
  Key4_G,
  Key4_B,
  Key5,
  Key5_R = Key5,
  Key5_G,
  Key5_B,
  Key6,
  Key6_R = Key6,
  Key6_G,
  Key6_B,
  Key7,
  Key7_R = Key7,
  Key7_G,
  Key7_B,
  Key8,
  Key8_R = Key8,
  Key8_G,
  Key8_B,
  Key9,
  Key9_R = Key9,
  Key9_G,
  Key9_B,
  Key10,
  Key10_R = Key10,
  Key10_G,
  Key10_B,
  Key11,
  Key11_R = Key11,
  Key11_G,
  Key11_B,
  Key12,
  Key12_R = Key12,
  Key12_G,
  Key12_B,
  Key13,
  Key13_R = Key13,
  Key13_G,
  Key13_B,
  Key14,
  Key14_R = Key14,
  Key14_G,
  Key14_B,
  Key15,
  Key15_R = Key15,
  Key15_G,
  Key15_B,
  Key16,
  Key16_R = Key16,
  Key16_G,
  Key16_B,
  Key17,
  Key17_R = Key17,
  Key17_G,
  Key17_B,
  Key18,
  Key18_R = Key18,
  Key18_G,
  Key18_B,
  Key19,
  Key19_R = Key19,
  Key19_G,
  Key19_B,
  Key20,
  Key20_R = Key20,
  Key20_G,
  Key20_B,
  Key21,
  Key21_R = Key21,
  Key21_G,
  Key21_B,
  Key22,
  Key22_R = Key22,
  Key22_G,
  Key22_B,
  Key23,
  Key23_R = Key23,
  Key23_G,
  Key23_B,
  Key24,
  Key24_R = Key24,
  Key24_G,
  Key24_B,
  Key25,
  Key25_R = Key25,
  Key25_G,
  Key25_B,
  Key26,
  Key26_R = Key26,
  Key26_G,
  Key26_B,
  Key27,
  Key27_R = Key27,
  Key27_G,
  Key27_B,
  Key28,
  Key28_R = Key28,
  Key28_G,
  Key28_B,
  Key29,
  Key29_R = Key29,
  Key29_G,
  Key29_B,
  Key30,
  Key30_R = Key30,
  Key30_G,
  Key30_B,
  Key31,
  Key31_R = Key31,
  Key31_G,
  Key31_B,
  Key32,
  Key32_R = Key32,
  Key32_G,
  Key32_B,
  Key33,
  Key33_R = Key33,
  Key33_G,
  Key33_B,
  Key34,
  Key34_R = Key34,
  Key34_G,
  Key34_B,
  Key35,
  Key35_R = Key35,
  Key35_G,
  Key35_B,
  Key36,
  Key36_R = Key36,
  Key36_G,
  Key36_B,
  Key37,
  Key37_R = Key37,
  Key37_G,
  Key37_B,
  Key38,
  Key38_R = Key38,
  Key38_G,
  Key38_B,
  Key39,
  Key39_R = Key39,
  Key39_G,
  Key39_B,
  Key40,
  Key40_R = Key40,
  Key40_G,
  Key40_B,
  Key41,
  Key41_R = Key41,
  Key41_G,
  Key41_B,
  Key42,
  Key42_R = Key42,
  Key42_G,
  Key42_B,
  Key43,
  Key43_R = Key43,
  Key43_G,
  Key43_B,
  Key44,
  Key44_R = Key44,
  Key44_G,
  Key44_B,
  Key45,
  Key45_R = Key45,
  Key45_G,
  Key45_B,
  Key46,
  Key46_R = Key46,
  Key46_G,
  Key46_B,
  Key47,
  Key47_R = Key47,
  Key47_G,
  Key47_B,
  Key48,
  Key48_R = Key48,
  Key48_G,
  Key48_B,
  Key49,
  Key49_R = Key49,
  Key49_G,
  Key49_B,
  Key50,
  Key50_R = Key50,
  Key50_G,
  Key50_B,
  Key51,
  Key51_R = Key51,
  Key51_G,
  Key51_B,
  Key52,
  Key52_R = Key52,
  Key52_G,
  Key52_B,
  Key53,
  Key53_R = Key53,
  Key53_G,
  Key53_B,
  Key54,
  Key54_R = Key54,
  Key54_G,
  Key54_B,
  Key55,
  Key55_R = Key55,
  Key55_G,
  Key55_B,
  Key56,
  Key56_R = Key56,
  Key56_G,
  Key56_B,
  Key57,
  Key57_R = Key57,
  Key57_G,
  Key57_B,
  Key58,
  Key58_R = Key58,
  Key58_G,
  Key58_B,
  Key59,
  Key59_R = Key59,
  Key59_G,
  Key59_B,
  Key60,
  Key60_R = Key60,
  Key60_G,
  Key60_B,
  Key61,
  Key61_R = Key61,
  Key61_G,
  Key61_B,
  Key62,
  Key62_R = Key62,
  Key62_G,
  Key62_B,
  Key63,
  Key63_R = Key63,
  Key63_G,
  Key63_B,
  Key64,
  Key64_R = Key64,
  Key64_G,
  Key64_B,
  Key65,
  Key65_R = Key65,
  Key65_G,
  Key65_B,
  Key66,
  Key66_R = Key66,
  Key66_G,
  Key66_B,
  Key67,
  Key67_R = Key67,
  Key67_G,
  Key67_B,
  Key68,
  Key68_R = Key68,
  Key68_G,
  Key68_B,
  Key69,
  Key69_R = Key69,
  Key69_G,
  Key69_B,
  Key70,
  Key70_R = Key70,
  Key70_G,
  Key70_B,
  Key71,
  Key71_R = Key71,
  Key71_G,
  Key71_B,
  Key72,
  Key72_R = Key72,
  Key72_G,
  Key72_B,
  Key73,
  Key73_R = Key73,
  Key73_G,
  Key73_B,
  Key74,
  Key74_R = Key74,
  Key74_G,
  Key74_B,
  Key75,
  Key75_R = Key75,
  Key75_G,
  Key75_B,
  Key76,
  Key76_R = Key76,
  Key76_G,
  Key76_B,
  Key77,
  Key77_R = Key77,
  Key77_G,
  Key77_B,
  Key78,
  Key78_R = Key78,
  Key78_G,
  Key78_B,
  Key79,
  Key79_R = Key79,
  Key79_G,
  Key79_B,
  Key80,
  Key80_R = Key80,
  Key80_G,
  Key80_B,
  Key81,
  Key81_R = Key81,
  Key81_G,
  Key81_B,
  Key82,
  Key82_R = Key82,
  Key82_G,
  Key82_B,
  Key83,
  Key83_R = Key83,
  Key83_G,
  Key83_B,
  Key84,
  Key84_R = Key84,
  Key84_G,
  Key84_B,
  Key85,
  Key85_R = Key85,
  Key85_G,
  Key85_B,
  Key86,
  Key86_R = Key86,
  Key86_G,
  Key86_B,
  Key87,
  Key87_R = Key87,
  Key87_G,
  Key87_B,
  Key88,
  Key88_R = Key88,
  Key88_G,
  Key88_B,
  Key89,
  Key89_R = Key89,
  Key89_G,
  Key89_B,
  Key90,
  Key90_R = Key90,
  Key90_G,
  Key90_B,
  Key91,
  Key91_R = Key91,
  Key91_G,
  Key91_B,
  Key92,
  Key92_R = Key92,
  Key92_G,
  Key92_B,
  Key93,
  Key93_R = Key93,
  Key93_G,
  Key93_B,
  Key94,
  Key94_R = Key94,
  Key94_G,
  Key94_B,
  Key95,
  Key95_R = Key95,
  Key95_G,
  Key95_B,
  Key96,
  Key96_R = Key96,
  Key96_G,
  Key96_B,
  Key97,
  Key97_R = Key97,
  Key97_G,
  Key97_B,
  Key98,
  Key98_R = Key98,
  Key98_G,
  Key98_B,
  Key99,
  Key99_R = Key99,
  Key99_G,
  Key99_B,
  Key100,
  Key100_R = Key100,
  Key100_G,
  Key100_B,
  Key101,
  Key101_R = Key101,
  Key101_G,
  Key101_B,
  Key102,
  Key102_R = Key102,
  Key102_G,
  Key102_B,
  Key103,
  Key103_R = Key103,
  Key103_G,
  Key103_B,
  Key104,
  Key104_R = Key104,
  Key104_G,
  Key104_B,
  Key105,
  Key105_R = Key105,
  Key105_G,
  Key105_B,
  Key106,
  Key106_R = Key106,
  Key106_G,
  Key106_B,
  Key107,
  Key107_R = Key107,
  Key107_G,
  Key107_B,
  Key108,
  Key108_R = Key108,
  Key108_G,
  Key108_B,
  Key109,
  Key109_R = Key109,
  Key109_G,
  Key109_B,
  Key110,
  Key110_R = Key110,
  Key110_G,
  Key110_B,
  Key111,
  Key111_R = Key111,
  Key111_G,
  Key111_B,
  Key112,
  Key112_R = Key112,
  Key112_G,
  Key112_B,
  Key113,
  Key113_R = Key113,
  Key113_G,
  Key113_B,
  Key114,
  Key114_R = Key114,
  Key114_G,
  Key114_B,
  Key115,
  Key115_R = Key115,
  Key115_G,
  Key115_B,
  Key116,
  Key116_R = Key116,
  Key116_G,
  Key116_B,
  Key117,
  Key117_R = Key117,
  Key117_G,
  Key117_B,
  Key118,
  Key118_R = Key118,
  Key118_G,
  Key118_B,
  Key119,
  Key119_R = Key119,
  Key119_G,
  Key119_B,
  Key120,
  Key120_R = Key120,
  Key120_G,
  Key120_B,
  Key121,
  Key121_R = Key121,
  Key121_G,
  Key121_B,
  Key122,
  Key122_R = Key122,
  Key122_G,
  Key122_B,
  Key123,
  Key123_R = Key123,
  Key123_G,
  Key123_B,
  Key124,
  Key124_R = Key124,
  Key124_G,
  Key124_B,
  Key125,
  Key125_R = Key125,
  Key125_G,
  Key125_B,
  Key126,
  Key126_R = Key126,
  Key126_G,
  Key126_B,
  Key127,
  Key127_R = Key127,
  Key127_G,
  Key127_B,
  Key128,
  Key128_R = Key128,
  Key128_G,
  Key128_B,

  Unknown,

};

//--------------------------------------------------------------------------------------------------

enum class KompleteKontrol::Button : uint8_t
{
  MainEncoder,
  PresetUp,
  Enter,
  PresetDown,
  Browse,
  Instance,
  OctaveDown,
  OctaveUp,

  Stop,
  Rec,
  Play,
  NavigateRight,
  NavigateDown,
  NavigateLeft,
  Back,
  NavigateUp,

  Shift,
  Scale,
  Arp,
  Loop,
  PageRight,
  PageLeft,
  Rwd,
  Ffw,

  TouchEncoder1,
  TouchEncoder2,
  TouchEncoder3,
  TouchEncoder4,
  TouchEncoder5,
  TouchEncoder6,
  TouchEncoder7,
  TouchEncoder8,

  TouchEncoderMain,

  None,
};

//--------------------------------------------------------------------------------------------------

KompleteKontrol::KompleteKontrol(tPtr<DeviceHandle> pDeviceHandle_, uint8_t numKeys_)
  : Device(std::move(pDeviceHandle_))
  , m_numKeys(numKeys_)
  , m_ledKeysDataSize(numKeys_ * 3)
  , m_isDirtyLeds(true)
  , m_isDirtyKeyLeds(true)
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  , m_pMidiOut(new RtMidiOut)
  , m_pMidiIn(new RtMidiIn)
#endif
{
  m_buttons.resize(kKK_buttonsDataSize);
  m_leds.resize(kKK_ledsDataSize);
  m_ledsKeys.resize(m_ledKeysDataSize);

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  std::string portName;
  unsigned nPorts = m_pMidiOut->getPortCount();
  for (unsigned int i = 0; i < nPorts; i++)
  {
    try
    {
      portName = m_pMidiOut->getPortName(i);
      if (portName.find(kKK_midiPortName) != std::string::npos && portName.back() == '1')
      {
        m_pMidiOut->openPort(i);
      }
    }
    catch (RtMidiError& error)
    {
      M_LOG("[MaschineMK2] RtMidiError: " << error.getMessage());
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
      if (portName.find(kKK_midiPortName) != std::string::npos && portName.back() == '1')
      {
        m_pMidiIn->openPort(i);
      }
    }
    catch (RtMidiError& error)
    {
      M_LOG("[MaschineMK2] RtMidiError: " << error.getMessage());
    }
  }
  if (!m_pMidiIn->isPortOpen())
  {
    m_pMidiIn.reset(nullptr);
  }
  else
  {
    m_pMidiIn->setCallback(&KompleteKontrol::midiInCallback, this);
  }
#endif
}

//--------------------------------------------------------------------------------------------------

KompleteKontrol::~KompleteKontrol()
{
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  m_pMidiOut->closePort();
  m_pMidiIn->closePort();
#endif
}

//--------------------------------------------------------------------------------------------------

void KompleteKontrol::setLed(Device::Button btn_, const util::LedColor& color_)
{
  setLedImpl(getLed(btn_), color_);
}

//--------------------------------------------------------------------------------------------------

void KompleteKontrol::setLed(Device::Key key_, const util::LedColor& color_)
{
  setLedImpl(getLed(key_), color_);
}

//--------------------------------------------------------------------------------------------------

void KompleteKontrol::sendMidiMsg(tRawData midiMsg_)
{
  //!\todo Use KompleteKontrol hardware midi port
}

//--------------------------------------------------------------------------------------------------

GDisplay* KompleteKontrol::getGraphicDisplay(uint8_t displayIndex_)
{
  return &m_displayDummy;
}

//--------------------------------------------------------------------------------------------------

LCDDisplay* KompleteKontrol::getLCDDisplay(uint8_t displayIndex_)
{
  static LCDDisplay s_dummyLCDDisplay(0, 0);
  if (displayIndex_ > 8)
  {
    return &s_dummyLCDDisplay;
  }
  return &m_displays[displayIndex_];
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::tick()
{
  static int state = 0;
  bool success     = false;

  if (state == 0)
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

void KompleteKontrol::init()
{
  getDeviceHandle()->write(Transfer({0xA0, 0x00, 0x00}), kKK_epOut);
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::sendDisplayData()
{
  bool result = true;

  tRawData displayData(240);

  for (uint8_t row = 0; row < 3; row++)
  {
    for (uint8_t i = 0; i < kKK_nDisplays; i++)
    {
      if (m_displays[i].isDirty())
      {
        std::copy_n(m_displays[i].getData().data() + (row * 16), 16, &displayData[i * 16]);
      }
    }
    if (!getDeviceHandle()->write(
          Transfer({0xe0, 0x00, 0x00, row, 0x00, 0x48, 0x00, 0x01, 0x00}, displayData), kKK_epOut))
    {
      result = false;
    }
  }

  return result;
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::sendLeds()
{
  if (m_isDirtyLeds)
  {
    if (!getDeviceHandle()->write(Transfer({0x80}, &m_leds[0], kKK_ledsDataSize), kKK_epOut))
    {
      return false;
    }
    m_isDirtyLeds = false;
  }
  if (m_isDirtyKeyLeds)
  {
    if (!getDeviceHandle()->write(Transfer({0x82}, &m_ledsKeys[0], m_ledKeysDataSize), kKK_epOut))
    {
      return false;
    }
    m_isDirtyKeyLeds = false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::read()
{
  Transfer input;

  if (!getDeviceHandle()->read(input, kKK_epInput))
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

void KompleteKontrol::processButtons(const Transfer& input_)
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
  }

  // Now process the encoder data
  uint8_t currentEncoderValue = input_.getData()[kKK_buttonsDataSize];
  if (currentEncoderValue != m_encoderValues[0])
  {
    bool valueIncreased = ((m_encoderValues[0] < currentEncoderValue)
                            || ((m_encoderValues[0] == 0x0f) && (currentEncoderValue == 0x00)))
                          && (!((m_encoderValues[0] == 0x0) && (currentEncoderValue == 0x0f)));
    m_encoderValues[0] = currentEncoderValue;
    encoderChanged(Device::Encoder::Main, valueIncreased, shiftPressed);
  }

  for (uint8_t encIndex = 0, i = kKK_buttonsDataSize + 1; encIndex < 8; i += 2, encIndex++)
  {
    Device::Encoder encoder
      = static_cast<Device::Encoder>(static_cast<uint8_t>(Device::Encoder::Encoder1) + encIndex);
    uint16_t value  = (input_.getData()[i]) | (input_.getData()[i + 1] << 8);
    uint16_t hValue = input_.getData()[i + 1];
    if (m_encoderValues[encIndex + 1] != value)
    {
      uint16_t prevHValue = (m_encoderValues[encIndex + 1] & 0xF00) >> 8;
      bool valueIncreased
        = ((m_encoderValues[encIndex + 1] < value) || ((prevHValue == 3) && (hValue == 0)))
          && (!((prevHValue == 0) && (hValue == 3)));
      m_encoderValues[encIndex + 1] = value;
      encoderChanged(encoder, valueIncreased, shiftPressed);
    }
  }

  m_firstOctave = input_.getData()[37];
}

//--------------------------------------------------------------------------------------------------

void KompleteKontrol::setLedImpl(Led led_, const util::LedColor& color_)
{
  static const uint8_t kFirstKeyIndex = static_cast<uint16_t>(Led::Key1);
  uint16_t ledIndex                   = static_cast<uint16_t>(led_);

  if (Led::Unknown == led_)
  {
    return;
  }

  if (isRGBLed(led_))
  {
    uint8_t currentR = m_ledsKeys[ledIndex - kFirstKeyIndex];
    uint8_t currentG = m_ledsKeys[ledIndex - kFirstKeyIndex + 1];
    uint8_t currentB = m_ledsKeys[ledIndex - kFirstKeyIndex + 2];

    m_ledsKeys[ledIndex - kFirstKeyIndex]     = color_.getRed();
    m_ledsKeys[ledIndex - kFirstKeyIndex + 1] = color_.getGreen();
    m_ledsKeys[ledIndex - kFirstKeyIndex + 2] = color_.getBlue();

    m_isDirtyKeyLeds
      = m_isDirtyKeyLeds || (currentR != color_.getRed() || currentG != color_.getGreen()
                              || currentB != color_.getBlue());
  }
  else
  {
    uint8_t currentVal = m_leds[ledIndex];
    uint8_t newVal     = color_.getMono();

    m_leds[ledIndex] = newVal;
    m_isDirtyLeds    = m_isDirtyLeds || (currentVal != newVal);
  }
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::isRGBLed(Led led_) const noexcept
{
  if (led_ >= Led::Key1)
  {
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

KompleteKontrol::Led KompleteKontrol::getLed(Device::Button btn_) const noexcept
{
#define M_LED_CASE(idLed)     \
  case Device::Button::idLed: \
    return Led::idLed

  switch (btn_)
  {
    M_LED_CASE(Shift);
    M_LED_CASE(Scale);
    M_LED_CASE(Arp);
    M_LED_CASE(Loop);
    M_LED_CASE(Rwd);
    M_LED_CASE(Ffw);
    M_LED_CASE(Play);
    M_LED_CASE(Rec);
    M_LED_CASE(Stop);
    M_LED_CASE(PageLeft);
    M_LED_CASE(PageRight);
    M_LED_CASE(Browse);
    M_LED_CASE(PresetUp);
    M_LED_CASE(Instance);
    M_LED_CASE(PresetDown);
    M_LED_CASE(Back);
    M_LED_CASE(NavigateUp);
    M_LED_CASE(Enter);
    M_LED_CASE(NavigateLeft);
    M_LED_CASE(NavigateDown);
    M_LED_CASE(NavigateRight);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

KompleteKontrol::Led KompleteKontrol::getLed(Device::Key key_) const noexcept
{
  static const Device::Key kMaxKey = static_cast<Device::Key>(m_numKeys);
  if (key_ >= kMaxKey)
    return Led::Unknown;

#define M_KEY_CASE(idKey)  \
  case Device::Key::idKey: \
    return Led::idKey

  switch (key_)
  {
    M_KEY_CASE(Key1);
    M_KEY_CASE(Key2);
    M_KEY_CASE(Key3);
    M_KEY_CASE(Key4);
    M_KEY_CASE(Key5);
    M_KEY_CASE(Key6);
    M_KEY_CASE(Key7);
    M_KEY_CASE(Key8);
    M_KEY_CASE(Key9);
    M_KEY_CASE(Key10);
    M_KEY_CASE(Key11);
    M_KEY_CASE(Key12);
    M_KEY_CASE(Key13);
    M_KEY_CASE(Key14);
    M_KEY_CASE(Key15);
    M_KEY_CASE(Key16);
    M_KEY_CASE(Key17);
    M_KEY_CASE(Key18);
    M_KEY_CASE(Key19);
    M_KEY_CASE(Key20);
    M_KEY_CASE(Key21);
    M_KEY_CASE(Key22);
    M_KEY_CASE(Key23);
    M_KEY_CASE(Key24);
    M_KEY_CASE(Key25);
    M_KEY_CASE(Key26);
    M_KEY_CASE(Key27);
    M_KEY_CASE(Key28);
    M_KEY_CASE(Key29);
    M_KEY_CASE(Key30);
    M_KEY_CASE(Key31);
    M_KEY_CASE(Key32);
    M_KEY_CASE(Key33);
    M_KEY_CASE(Key34);
    M_KEY_CASE(Key35);
    M_KEY_CASE(Key36);
    M_KEY_CASE(Key37);
    M_KEY_CASE(Key38);
    M_KEY_CASE(Key39);
    M_KEY_CASE(Key40);
    M_KEY_CASE(Key41);
    M_KEY_CASE(Key42);
    M_KEY_CASE(Key43);
    M_KEY_CASE(Key44);
    M_KEY_CASE(Key45);
    M_KEY_CASE(Key46);
    M_KEY_CASE(Key47);
    M_KEY_CASE(Key48);
    M_KEY_CASE(Key49);
    M_KEY_CASE(Key50);
    M_KEY_CASE(Key51);
    M_KEY_CASE(Key52);
    M_KEY_CASE(Key53);
    M_KEY_CASE(Key54);
    M_KEY_CASE(Key55);
    M_KEY_CASE(Key56);
    M_KEY_CASE(Key57);
    M_KEY_CASE(Key58);
    M_KEY_CASE(Key59);
    M_KEY_CASE(Key60);
    M_KEY_CASE(Key61);
    M_KEY_CASE(Key62);
    M_KEY_CASE(Key63);
    M_KEY_CASE(Key64);
    M_KEY_CASE(Key65);
    M_KEY_CASE(Key66);
    M_KEY_CASE(Key67);
    M_KEY_CASE(Key68);
    M_KEY_CASE(Key69);
    M_KEY_CASE(Key70);
    M_KEY_CASE(Key71);
    M_KEY_CASE(Key72);
    M_KEY_CASE(Key73);
    M_KEY_CASE(Key74);
    M_KEY_CASE(Key75);
    M_KEY_CASE(Key76);
    M_KEY_CASE(Key77);
    M_KEY_CASE(Key78);
    M_KEY_CASE(Key79);
    M_KEY_CASE(Key80);
    M_KEY_CASE(Key81);
    M_KEY_CASE(Key82);
    M_KEY_CASE(Key83);
    M_KEY_CASE(Key84);
    M_KEY_CASE(Key85);
    M_KEY_CASE(Key86);
    M_KEY_CASE(Key87);
    M_KEY_CASE(Key88);
    M_KEY_CASE(Key89);
    M_KEY_CASE(Key90);
    M_KEY_CASE(Key91);
    M_KEY_CASE(Key92);
    M_KEY_CASE(Key93);
    M_KEY_CASE(Key94);
    M_KEY_CASE(Key95);
    M_KEY_CASE(Key96);
    M_KEY_CASE(Key97);
    M_KEY_CASE(Key98);
    M_KEY_CASE(Key99);
    M_KEY_CASE(Key100);
    M_KEY_CASE(Key101);
    M_KEY_CASE(Key102);
    M_KEY_CASE(Key103);
    M_KEY_CASE(Key104);
    M_KEY_CASE(Key105);
    M_KEY_CASE(Key106);
    M_KEY_CASE(Key107);
    M_KEY_CASE(Key108);
    M_KEY_CASE(Key109);
    M_KEY_CASE(Key110);
    M_KEY_CASE(Key111);
    M_KEY_CASE(Key112);
    M_KEY_CASE(Key113);
    M_KEY_CASE(Key114);
    M_KEY_CASE(Key115);
    M_KEY_CASE(Key116);
    M_KEY_CASE(Key117);
    M_KEY_CASE(Key118);
    M_KEY_CASE(Key119);
    M_KEY_CASE(Key120);
    M_KEY_CASE(Key121);
    M_KEY_CASE(Key122);
    M_KEY_CASE(Key123);
    M_KEY_CASE(Key124);
    M_KEY_CASE(Key125);
    M_KEY_CASE(Key126);
    M_KEY_CASE(Key127);
    M_KEY_CASE(Key128);
    default:
    {
      return Led::Unknown;
    }
  }

#undef M_PAD_CASE
}

//--------------------------------------------------------------------------------------------------

Device::Button KompleteKontrol::getDeviceButton(Button btn_) const noexcept
{
#define M_BTN_CASE(idBtn) \
  case Button::idBtn:     \
    return Device::Button::idBtn

  switch (btn_)
  {
    M_BTN_CASE(MainEncoder);
    M_BTN_CASE(PresetUp);
    M_BTN_CASE(Enter);
    M_BTN_CASE(PresetDown);
    M_BTN_CASE(Browse);
    M_BTN_CASE(Instance);
    M_BTN_CASE(OctaveDown);
    M_BTN_CASE(OctaveUp);
    M_BTN_CASE(Stop);
    M_BTN_CASE(Rec);
    M_BTN_CASE(Play);
    M_BTN_CASE(NavigateRight);
    M_BTN_CASE(NavigateDown);
    M_BTN_CASE(NavigateLeft);
    M_BTN_CASE(Back);
    M_BTN_CASE(NavigateUp);
    M_BTN_CASE(Shift);
    M_BTN_CASE(Scale);
    M_BTN_CASE(Arp);
    M_BTN_CASE(Loop);
    M_BTN_CASE(PageRight);
    M_BTN_CASE(PageLeft);
    M_BTN_CASE(Rwd);
    M_BTN_CASE(Ffw);
    default:
    {
      return Device::Button::Unknown;
    }
  }

#undef M_LED_CASE
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::isButtonPressed(Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((m_buttons[buttonPos >> 3] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

bool KompleteKontrol::isButtonPressed(const Transfer& transfer_, Button button_) const noexcept
{
  uint8_t buttonPos = static_cast<uint8_t>(button_);
  return ((transfer_[1 + (buttonPos >> 3)] & (1 << (buttonPos % 8))) != 0);
}

//--------------------------------------------------------------------------------------------------

void KompleteKontrol::midiInCallback(
  double timeStamp_, std::vector<unsigned char>* pMessage_, void* userData_)
{
  KompleteKontrol* pSelf = static_cast<KompleteKontrol*>(userData_);
  if ((pMessage_->at(0) & 0xf0) == 0x90)
  {
    pSelf->keyChanged(static_cast<Device::Key>(pMessage_->at(1) - pSelf->m_firstOctave),
      pMessage_->at(2),
      pSelf->isButtonPressed(Button::Shift));
  }
}

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
