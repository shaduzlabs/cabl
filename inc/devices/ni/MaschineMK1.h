/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <bitset>

#include "devices/Device.h"
#include "gfx/displays/GDisplayMaschineMK1.h"
#include "comm/Transfer.h"

namespace sl
{
namespace cabl
{
namespace devices
{
 
//--------------------------------------------------------------------------------------------------

class MaschineMK1 : public Device<MaschineMK1>
{
   
public:
  
  MaschineMK1(tPtr<DeviceHandle>);
  ~MaschineMK1() override;
  
  void setLed(DeviceBase::Button, const util::LedColor&) override;
  void setLed(DeviceBase::Pad, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;

  GDisplay* getGraphicDisplay( uint8_t displayIndex_ ) override;
  LCDDisplay* getLCDDisplay(uint8_t displayIndex_) override;

  bool tick() override;

private:

  enum class Led : uint8_t;
  enum class Button : uint8_t;
  enum class Encoder : uint8_t;

  static constexpr uint8_t kMASMK1_nDisplays      = 2;
  static constexpr uint8_t kMASMK1_ledsDataSize   = 62;
  static constexpr uint8_t kMASMK1_nButtons = 41;
  static constexpr uint8_t kMASMK1_buttonsDataSize = 7;
  static constexpr uint8_t kMASMK1_padDataSize = 64;
  static constexpr uint8_t kMASMK1_padsBufferSize = 16;
  static constexpr uint8_t kMASMK1_nPads = 16;

  static constexpr uint8_t kMASMK1_nEncoders = 11;

  void init() override;

  void initDisplay( uint8_t displayIndex_ );
  bool sendFrame( uint8_t displayIndex_ );
  bool sendLeds();
  bool read();

  void processPads(const Transfer&);
  void processButtons(const Transfer&);
  void processEncoders(const Transfer&);

  void setLedImpl(Led, const util::LedColor&);
  Led getLed(DeviceBase::Button) const noexcept;
  Led getLed(DeviceBase::Pad) const noexcept;

  DeviceBase::Button getDeviceButton(Button btn_) const noexcept;
  DeviceBase::Encoder getDeviceEncoder(Encoder btn_) const noexcept;
  
  void cbRead(Transfer);
    
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  GDisplayMaschineMK1 m_displays[kMASMK1_nDisplays];
  tRawData            m_leds;
  tRawData            m_buttons;

  bool                m_buttonStates[kMASMK1_nButtons];
  uint16_t             m_encoderValues[kMASMK1_nEncoders];

  uint16_t                    m_padsData[ kMASMK1_nPads ];
  std::bitset<kMASMK1_nPads>  m_padsStatus;

  bool                m_isDirtyLedGroup0;
  bool                m_isDirtyLedGroup1;
  bool                m_encodersInitialized;
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
