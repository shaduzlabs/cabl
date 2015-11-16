/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <bitset>

#include "devices/Device.h"
#include "gfx/displays/GDisplayMaschineMikro.h"

namespace sl
{
namespace cabl
{
namespace devices
{
  
//--------------------------------------------------------------------------------------------------

class MaschineMikroMK2 : public Device<MaschineMikroMK2>
{
 
public:
  
  MaschineMikroMK2(tPtr<DeviceHandle>);
  ~MaschineMikroMK2() override;
  
  void setLed(DeviceBase::Button, const util::LedColor&) override;
  void setLed(DeviceBase::Pad, const util::LedColor&) override;

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

  void init() override;

  void initDisplay() const;
  bool sendFrame();
  bool sendLeds();
  bool read();
  
  void processButtons( const Transfer& );
  void processPads( const Transfer& );
  
  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led getLed(DeviceBase::Button) const noexcept;
  Led getLed(DeviceBase::Pad) const noexcept;

  DeviceBase::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMikro m_display;
  
  tRawData              m_leds;
  tRawData              m_buttons;
  bool                  m_buttonStates[kMikroMK2_nButtons];
  uint8_t               m_encoderValue;
  
  uint16_t                      m_padsData[ kMikroMK2_nPads ];
  std::bitset<kMikroMK2_nPads>  m_padsStatus;
  
  bool                  m_isDirtyLeds;

};
  
//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
