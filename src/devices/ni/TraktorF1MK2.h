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
#include "gfx/displays/GDisplayDummy.h"
#include "gfx/displays/LCDDisplay7Segments.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class TraktorF1MK2 : public Device
{

public:
  TraktorF1MK2();

  void setLed(Device::Button, const util::ColorRGB&) override;
  void setLed(Device::Pad, const util::ColorRGB&) override;

  LCDDisplay* displayLCD(size_t displayIndex_) override;

  size_t numOfGraphicDisplays() const override
  {
    return 0;
  }

  size_t numOfLCDDisplays() const override
  {
    return 1;
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
  enum class Led : uint16_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kF1MK2_nButtons = 27;
  static constexpr uint8_t kF1MK2_buttonsDataSize = 5;
  static constexpr uint8_t kF1MK2_nPotentiometers = 8;
  static constexpr uint8_t kF1MK2_nLeds = 80;

  void init() override;
  bool sendLedsAndDisplay();
  bool read();

  void processButtons(const Transfer&);

  void setLedImpl(Led, const util::ColorRGB&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(Device::Pad) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayDummy m_displayDummy;
  LCDDisplay7Segments<2> m_lcdDisplay;

  std::array<uint8_t, kF1MK2_buttonsDataSize> m_buttons;
  std::array<uint8_t, kF1MK2_nLeds> m_leds;

  std::bitset<kF1MK2_nButtons> m_buttonStates;
  uint16_t m_potentiometersValues[kF1MK2_nPotentiometers];
  uint8_t m_encoderValue;

  bool m_isDirtyLeds;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(TraktorF1MK2, "", DeviceDescriptor::Type::HID, 0x17CC, 0x1120);

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
