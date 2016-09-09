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
#include "gfx/LedArray.h"
#include "gfx/LedMatrix.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class MaschineJam : public Device
{

public:
  MaschineJam();

  void setLed(Device::Button, const util::ColorRGB&) override;
  void setLed(Device::Pad, const util::ColorRGB&) override;

  LedMatrix* ledMatrix(size_t ledMatrixIndex_) override;
  LedArray* ledArray(size_t ledArrayIndex_) override;

  size_t numOfGraphicDisplays() const override { return 0; }

  size_t numOfLCDDisplays() const override { return 0; }

  size_t numOfLedMatrices() const override { return 1; }

  size_t numOfLedArrays() const override { return 10; }

  bool tick() override;
  
private:
  enum class Led : uint8_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kMASJ_nButtons = 117;
  static constexpr uint8_t kMASJ_buttonsDataSize = 17;
  static constexpr uint8_t kMASJ_nPads = 64;
  static constexpr uint8_t kMASJ_padsBufferSize = 16;
  static constexpr uint8_t kMASJ_nLedsButtons = 41;
  static constexpr uint8_t kMASJ_nLedsPads = 80;
  static constexpr uint8_t kMASJ_nLedsStrips = 88;
  
  static constexpr uint8_t kMASJ_nLedArrays = 10;

  void init() override;

  bool sendLeds();
  bool read();

  void processButtons(const Transfer&);
  void processStrips(const Transfer&);

  void setLedImpl(Led, const util::ColorRGB&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(Device::Pad) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  std::array<uint8_t, kMASJ_nLedsButtons> m_ledsButtons;
  std::array<uint8_t, kMASJ_nLedsPads> m_ledsPads;
  std::array<uint8_t, kMASJ_nLedsStrips> m_ledsStrips;
  std::array<uint8_t, kMASJ_buttonsDataSize> m_buttons;

  LedMatrix m_ledMatrix;
  std::vector<LedArray> m_ledArrays;
  std::bitset<kMASJ_nButtons> m_buttonStates;
  std::bitset<kMASJ_nPads> m_padsStatus;
  uint8_t m_encoderValue;

  mutable bool m_isDirtyPadLeds{false};
  mutable bool m_isDirtyStripLeds{false};
  mutable bool m_isDirtyButtonLeds{false};
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(MaschineJam, "", DeviceDescriptor::Type::HID, 0x17CC, 0x1500);

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
