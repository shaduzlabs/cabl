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
#include "cabl/gfx/LedArray.h"
#include "cabl/gfx/LedMatrix.h"
#include "gfx/LedArrayMaschineJam.h"
#include "gfx/displays/LedMatrixMaschineJam.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class MaschineJam : public Device
{

public:
  MaschineJam();

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  Canvas* ledMatrix(size_t ledMatrixIndex_) override;
  LedArray* ledArray(size_t ledArrayIndex_) override;

  size_t numOfGraphicDisplays() const override
  {
    return 0;
  }

  size_t numOfTextDisplays() const override
  {
    return 0;
  }

  size_t numOfLedMatrices() const override
  {
    return 1;
  }

  size_t numOfLedArrays() const override
  {
    return 10;
  }

  bool tick() override;

  static uint8_t toLedColor(const Color& color_);
  static Color fromLedColor(uint8_t color_);

private:
  enum class Led : uint8_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kMASJ_nButtons = 117;
  static constexpr uint8_t kMASJ_buttonsDataSize = 17;
  static constexpr uint8_t kMASJ_nPads = 64;
  static constexpr uint8_t kMASJ_padsBufferSize = 16;
  static constexpr uint8_t kMASJ_nLedsButtons = 53;
  static constexpr uint8_t kMASJ_nLedsPads = 80;
  static constexpr uint8_t kMASJ_nTouchStrips = 8;
  static constexpr uint8_t kMASJ_nLedsStrips = 88;

  static constexpr uint8_t kMASJ_nLedArrays = 10;

  void init() override;

  bool sendLeds();
  bool read();

  void processButtons(const Transfer&);
  void processStrips(const Transfer&);

  void setLedImpl(Led, const Color&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(unsigned) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  std::array<uint8_t, kMASJ_nLedsButtons> m_ledsButtons{};
  std::array<uint8_t, kMASJ_nLedsPads> m_ledsPads{};
  std::array<uint8_t, kMASJ_nLedsStrips> m_ledsStrips{};
  std::array<uint8_t, kMASJ_buttonsDataSize> m_buttons{};

  LedMatrixMaschineJam m_ledMatrix;
  std::array<LedArrayMaschineJam<11>, kMASJ_nTouchStrips> m_ledArraysStrips{};
  std::array<LedArrayMaschineJam<8>, 2> m_ledArraysLevel{};
  std::array<unsigned, kMASJ_nTouchStrips> m_touchstripsValues{};

  std::bitset<kMASJ_nButtons> m_buttonStates{};
  std::bitset<kMASJ_nPads> m_padsStatus{};
  uint8_t m_encoderValue;
  
  mutable bool m_isDirtyPadLeds{false};
  mutable bool m_isDirtyStripLeds{false};
  mutable bool m_isDirtyButtonLeds{false};
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(MaschineJam, "", DeviceDescriptor::Type::HID, 0x17CC, 0x1500);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
