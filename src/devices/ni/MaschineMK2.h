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
#include "gfx/displays/GDisplayMaschineMK2.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class MaschineMK2 : public Device
{

public:
  MaschineMK2();
  ~MaschineMK2() override;

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  void sendMidiMsg(tRawData) override;

  Canvas* graphicDisplay(size_t displayIndex_) override;

  size_t numOfGraphicDisplays() const override
  {
    return 2;
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

  static constexpr uint8_t kMASMK2_nDisplays = 2;
  static constexpr uint8_t kMASMK2_nButtons = 48;
  static constexpr uint8_t kMASMK2_buttonsDataSize = 8;
  static constexpr uint8_t kMASMK2_padDataSize = 64;
  static constexpr uint8_t kMASMK2_nPads = 16;
  static constexpr uint8_t kMASMK2_padsBufferSize = 16;
  static constexpr uint8_t kMASMK2_nEncoders = 9;
  static constexpr uint8_t kMASMK2_nLedsButtons = 32;
  static constexpr uint8_t kMASMK2_nLedsGroups = 57;
  static constexpr uint8_t kMASMK2_nLedsPads = 49;

  void init() override;

  void initDisplay() const;
  bool sendFrame(uint8_t displayIndex);
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

  GDisplayMaschineMK2 m_displays[kMASMK2_nDisplays];

  std::array<uint8_t, kMASMK2_nLedsButtons> m_ledsButtons;
  std::array<uint8_t, kMASMK2_nLedsGroups> m_ledsGroups;
  std::array<uint8_t, kMASMK2_nLedsPads> m_ledsPads;

  std::array<uint8_t, kMASMK2_buttonsDataSize> m_buttons;

  std::bitset<kMASMK2_nButtons> m_buttonStates;
  unsigned m_encoderValues[kMASMK2_nEncoders];

  unsigned m_padsData[kMASMK2_nPads];
  std::bitset<kMASMK2_nPads> m_padsStatus;

  bool m_isDirtyPadLeds;
  bool m_isDirtyGroupLeds;
  bool m_isDirtyButtonLeds;

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  tPtr<RtMidiOut> m_pMidiout;
#endif
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(MaschineMK2, "", DeviceDescriptor::Type::HID, 0x17CC, 0x1140);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
