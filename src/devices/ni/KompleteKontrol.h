/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <bitset>

#include "cabl/devices/Device.h"
#include "cabl/devices/DeviceFactory.h"
#include "gfx/displays/NullCanvas.h"
#include "gfx/displays/TextDisplayKompleteKontrol.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class KompleteKontrolBase : public Device
{

public:

  enum NUM_KEYS
  {
    KEYS_25 = 25,
    KEYS_49 = 49,
    KEYS_61 = 61,
    KEYS_88 = 88
  };

  KompleteKontrolBase(NUM_KEYS numKeys);
  ~KompleteKontrolBase() override;

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  void sendMidiMsg(tRawData) override;

  TextDisplay* textDisplay(size_t displayIndex_) override;

  unsigned numKeys() const
  {
    return m_numKeys;
  }

  size_t numOfGraphicDisplays() const override
  {
    return 0;
  }

  size_t numOfTextDisplays() const override
  {
    return 9;
  }

  size_t numOfLedMatrices() const override
  {
    return 0;
  }

  size_t numOfLedArrays() const override
  {
    return 0;
  }

  size_t currentOctave() const override
  {
    return m_firstOctave;
  }

  unsigned ledDataSize() const
  {
    return m_ledsKeysSize;
  }

  uint8_t* ledsKeysData()
  {
    return &m_ledsKeys[0];
  }

  bool tick() override;

private:
  enum class Led : unsigned;
  enum class Button : uint8_t;

  static constexpr uint8_t kKK_nButtons = 37;
  static constexpr uint8_t kKK_buttonsDataSize = 6;
  static constexpr uint8_t kKK_nEncoders = 9;
  static constexpr uint8_t kKK_nDisplays = 9;

  void init() override;
  bool sendDisplayData();
  bool sendLeds();
  bool read();

  void processButtons(const Transfer&);

  void setLedImpl(Led, const Color&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(unsigned) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  static void midiInCallback(double timeStamp, std::vector<unsigned char>* message, void* userData);

  NullCanvas m_displayDummy;
  tRawData m_leds;
  tRawData m_buttons;
  std::bitset<kKK_nButtons> m_buttonStates;
  unsigned m_numKeys;
  unsigned m_encoderValues[kKK_nEncoders];
  unsigned m_ledsKeysSize;
  uint8_t* m_ledsKeys;

  bool m_isDirtyLeds;
  bool m_isDirtyKeyLeds;

  uint8_t m_firstOctave;

  TextDisplayKompleteKontrol m_displays[kKK_nDisplays];

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  tPtr<RtMidiOut> m_pMidiOut;
  tPtr<RtMidiIn> m_pMidiIn;
#endif
};

//--------------------------------------------------------------------------------------------------

class KompleteKontrolS25 final : public KompleteKontrolBase
{
public:
  KompleteKontrolS25() : KompleteKontrolBase(KEYS_25) {}
};

class KompleteKontrolS49 final : public KompleteKontrolBase
{
public:
  KompleteKontrolS49() : KompleteKontrolBase(KEYS_49) {}
};

class KompleteKontrolS61 final : public KompleteKontrolBase
{
public:
  KompleteKontrolS61() : KompleteKontrolBase(KEYS_61) {}
};

class KompleteKontrolS88 final : public KompleteKontrolBase
{
public:
  KompleteKontrolS88() : KompleteKontrolBase(KEYS_88) {}
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  KompleteKontrolS25, "KOMPLETE KONTROL S25", DeviceDescriptor::Type::HID, 0x17CC, 0x1340);
M_REGISTER_DEVICE_CLASS(
  KompleteKontrolS49, "KOMPLETE KONTROL S49", DeviceDescriptor::Type::HID, 0x17CC, 0x1350);
M_REGISTER_DEVICE_CLASS(
  KompleteKontrolS61, "KOMPLETE KONTROL S61", DeviceDescriptor::Type::HID, 0x17CC, 0x1360);
M_REGISTER_DEVICE_CLASS(
  KompleteKontrolS88, "KOMPLETE KONTROL S88", DeviceDescriptor::Type::HID, 0x17CC, 0x1410);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
