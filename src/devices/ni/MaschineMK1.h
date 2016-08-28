/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <bitset>

#include "comm/Transfer.h"
#include "devices/Device.h"
#include "devices/DeviceFactory.h"
#include "gfx/displays/GDisplayMaschineMK1.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class MaschineMK1 : public Device
{

public:
  MaschineMK1();
  ~MaschineMK1() override;

  void setLed(Device::Button, const util::ColorRGB&) override;
  void setLed(Device::Pad, const util::ColorRGB&) override;

  void sendMidiMsg(tRawData) override;

  GDisplay* displayGraphic(size_t displayIndex_) override;
  LCDDisplay* displayLCD(size_t displayIndex_) override;

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

  void setLedImpl(Led, const util::ColorRGB&);
  Led led(Device::Button) const noexcept;
  Led led(Device::Pad) const noexcept;

  Device::Button deviceButton(Button btn_) const noexcept;
  Device::Encoder deviceEncoder(Encoder btn_) const noexcept;

  void cbRead(Transfer);

  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMK1 m_displays[kMASMK1_nDisplays];
  tRawData m_leds;
  tRawData m_buttons;

  std::bitset<kMASMK1_nButtons> m_buttonStates;
  uint16_t m_encoderValues[kMASMK1_nEncoders];

  uint16_t m_padsData[kMASMK1_nPads];
  std::bitset<kMASMK1_nPads> m_padsStatus;

  bool m_isDirtyLedGroup0{true};
  bool m_isDirtyLedGroup1{true};
  bool m_encodersInitialized{false};
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  MaschineMK1, "Maschine Controller", DeviceDescriptor::Type::USB, 0x17CC, 0x0808);

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
