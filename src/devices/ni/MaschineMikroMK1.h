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
#include "gfx/displays/GDisplayMaschineMikro.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class MaschineMikroMK1 : public Device
{

public:
  MaschineMikroMK1();

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  Canvas* graphicDisplay(size_t displayIndex_) override;

  size_t numOfGraphicDisplays() const override
  {
    return 1;
  }

  size_t numOfTextDisplays() const override
  {
    return 0;
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
  enum class Led : uint8_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kMikroMK1_nButtons = 52;
  static constexpr uint8_t kMikroMK1_ledsDataSize = 60;
  static constexpr uint8_t kMikroMK1_buttonsDataSize = 5;
  static constexpr uint8_t kMikroMK1_padDataSize = 64;
  static constexpr uint8_t kMikroMK1_nPads = 16;
  static constexpr uint8_t kMikroMK1_padsBufferSize = 16;

  void init() override;

  void initDisplay() const;
  bool sendFrame();
  bool sendLeds();
  bool read();

  void processButtons(const Transfer&);
  void processPads(const Transfer&);

  void setLedImpl(Led, const Color&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(unsigned) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMikro m_display;

  std::array<uint8_t, kMikroMK1_ledsDataSize> m_leds;
  std::array<uint8_t, kMikroMK1_buttonsDataSize> m_buttons;

  std::bitset<kMikroMK1_nButtons> m_buttonStates;
  uint8_t m_encoderValue;

  unsigned m_padsData[kMikroMK1_nPads];
  std::bitset<kMikroMK1_nPads> m_padsStatus;

  bool m_isDirtyLeds;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  MaschineMikroMK1, "Maschine Mikro", DeviceDescriptor::Type::HID, 0x17CC, 0x1110);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
