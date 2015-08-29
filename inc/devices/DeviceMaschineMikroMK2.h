/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Device.h"
#include "gfx/displays/GDisplayMaschineMikro.h"

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------

class DeviceMaschineMikroMK2 : public Device
{
 
public:
  
  DeviceMaschineMikroMK2(tPtr<DeviceHandle>);
  ~DeviceMaschineMikroMK2() override;
  
  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Pad, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;
  
  GDisplay* getGraphicDisplay(uint8_t displayIndex_) override;
  LCDDisplay* getLCDDisplay(uint8_t displayIndex_) override;

  bool tick() override;

private:

  enum class Led    : uint8_t;
  enum class Button : uint8_t;
 
  static constexpr uint8_t kMikroMK2_nButtons          = 45;
  static constexpr uint8_t kMikroMK2_ledsDataSize      = 78;
  static constexpr uint8_t kMikroMK2_buttonsDataSize   = 5;
  static constexpr uint8_t kMikroMK2_padDataSize       = 64;
  static constexpr uint8_t kMikroMK2_nPads             = 16;
  static constexpr uint8_t kMikroMK2_padsBufferSize    = 16;

  using tBuffer = util::CircularBuffer<uint16_t, kMikroMK2_padsBufferSize>;

  void init() override;

  void initDisplay() const;
  bool sendFrame();
  bool sendLeds();
  bool read();
  
  void processButtons( const Transfer& );
  void processPads( const Transfer& );
  
  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led getLed(Device::Button) const noexcept;
  Led getLed(Device::Pad) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMikro m_display;
  
  tRawData              m_leds;
  tRawData              m_buttons;
  bool                  m_buttonStates[kMikroMK2_nButtons];
  uint8_t               m_encoderValue;
  
  tBuffer               m_padsRawData[ kMikroMK2_nPads ];
  uint16_t              m_padsAvgData[ kMikroMK2_nPads ];
  
  bool                  m_isDirtyLeds;

};
  
//--------------------------------------------------------------------------------------------------
  
} // kio
} // sl
