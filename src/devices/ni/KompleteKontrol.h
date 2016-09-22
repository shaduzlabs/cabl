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
  KompleteKontrolBase();
  ~KompleteKontrolBase() override;

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  void sendMidiMsg(tRawData) override;

  TextDisplay* textDisplay(size_t displayIndex_) override;

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

  virtual unsigned numKeys() const = 0;
  virtual unsigned ledDataSize() const = 0;
  virtual uint8_t* ledsKeysData() = 0;

  static void midiInCallback(double timeStamp, std::vector<unsigned char>* message, void* userData);

  NullCanvas m_displayDummy;
  tRawData m_leds;
  tRawData m_buttons;
  std::bitset<kKK_nButtons> m_buttonStates;
  unsigned m_encoderValues[kKK_nEncoders];

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

template <uint8_t NKEYS>
class KompleteKontrol final : public KompleteKontrolBase
{
public:
  static constexpr unsigned kKK_keysLedDataSize = NKEYS * 3U;

  unsigned numKeys() const override
  {
    return NKEYS;
  }
  unsigned ledDataSize() const override
  {
    return kKK_keysLedDataSize;
  }

private:
  uint8_t* ledsKeysData() override
  {
    return &m_ledsKeys[0];
  }

  uint8_t m_ledsKeys[kKK_keysLedDataSize];
};

//--------------------------------------------------------------------------------------------------

using KompleteKontrolS25 = KompleteKontrol<25>;
using KompleteKontrolS49 = KompleteKontrol<49>;
using KompleteKontrolS61 = KompleteKontrol<61>;
using KompleteKontrolS88 = KompleteKontrol<88>;

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
