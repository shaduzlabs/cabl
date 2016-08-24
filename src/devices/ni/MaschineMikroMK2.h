/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <bitset>

#include "devices/Device.h"
#include "devices/DeviceFactory.h"
#include "gfx/displays/GDisplayMaschineMikro.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class MaschineMikroMK2 : public Device
{

public:
  MaschineMikroMK2();

  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Pad, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;

  GDisplay* displayGraphic(size_t displayIndex_) override;
  LCDDisplay* displayLCD(size_t displayIndex_) override;

  bool tick() override;

private:
  enum class Led : uint8_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kMikroMK2_nButtons = 45;
  static constexpr uint8_t kMikroMK2_ledsDataSize = 78;
  static constexpr uint8_t kMikroMK2_buttonsDataSize = 5;
  static constexpr uint8_t kMikroMK2_padDataSize = 64;
  static constexpr uint8_t kMikroMK2_nPads = 16;
  static constexpr uint8_t kMikroMK2_padsBufferSize = 16;

  void init() override;

  void initDisplay() const;
  bool sendFrame();
  bool sendLeds();
  bool read();

  void processButtons(const Transfer&);
  void processPads(const Transfer&);

  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(Device::Pad) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMikro m_display;

  std::array<uint8_t, kMikroMK2_ledsDataSize> m_leds;
  std::array<uint8_t, kMikroMK2_buttonsDataSize> m_buttons;

  std::bitset<kMikroMK2_nButtons> m_buttonStates;
  uint8_t m_encoderValue;

  uint16_t m_padsData[kMikroMK2_nPads];
  std::bitset<kMikroMK2_nPads> m_padsStatus;

  bool m_isDirtyLeds;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  MaschineMikroMK2, "Maschine Mikro MK2", DeviceDescriptor::Type::HID, 0x17CC, 0x1200);

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
