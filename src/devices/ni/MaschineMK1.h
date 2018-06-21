/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <bitset>

#include "cabl/comm/Transfer.h"
#include "cabl/devices/Device.h"
#include "gfx/displays/GDisplayMaschineMK1.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class MaschineMK1 : public Device
{

public:
  MaschineMK1();

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
  enum class Encoder : uint8_t;

  static constexpr uint8_t kMASMK1_nDisplays = 2;
  static constexpr uint8_t kMASMK1_ledsDataSize = 62;
  static constexpr uint8_t kMASMK1_nButtons = 42;
  static constexpr uint8_t kMASMK1_buttonsDataSize = 7;
  static constexpr uint8_t kMASMK1_padDataSize = 64;
  static constexpr uint8_t kMASMK1_padsBufferSize = 16;
  static constexpr uint8_t kMASMK1_nPads = 16;

  static constexpr uint8_t kMASMK1_nEncoders = 11;

  void init() override;

  void initDisplay(uint8_t displayIndex_);
  bool sendFrame(uint8_t displayIndex_);
  bool sendLeds();
  bool read();

  void processPads(const Transfer&);
  void processButtons(const Transfer&);
  void processEncoders(const Transfer&);

  void setLedImpl(Led, const Color&);
  Led led(Device::Button) const noexcept;
  Led led(unsigned) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;

  void cbRead(Transfer);

  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMK1 m_displays[kMASMK1_nDisplays];
  tRawData m_leds;
  tRawData m_buttons;

  std::bitset<kMASMK1_nButtons> m_buttonStates;
  unsigned m_encoderValues[kMASMK1_nEncoders];

  unsigned m_padsData[kMASMK1_nPads];
  std::bitset<kMASMK1_nPads> m_padsStatus;

  bool m_isDirtyLedGroup0{true};
  bool m_isDirtyLedGroup1{true};
  bool m_encodersInitialized{false};
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  MaschineMK1, "Maschine Controller", DeviceDescriptor::Type::USB, 0x17CC, 0x0808);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
