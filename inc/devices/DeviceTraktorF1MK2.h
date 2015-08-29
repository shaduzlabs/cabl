/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Device.h"
#include "gfx/displays/GDisplayDummy.h"
#include "gfx/displays/LCDDisplay7Segments.h"

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------
    
class DeviceTraktorF1MK2 : public Device
{
 
public:
  
  DeviceTraktorF1MK2(tPtr<DeviceHandle>);
  ~DeviceTraktorF1MK2() override;
  
  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Pad, const util::LedColor&) override;
  
  void sendMidiMsg(tRawData) override;
  
  GDisplay* getGraphicDisplay(uint8_t displayIndex_) override;
  LCDDisplay* getLCDDisplay(uint8_t displayIndex_) override;

  bool tick() override;

private:

  enum class Led    : uint16_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kF1MK2_nButtons = 27;
  static constexpr uint8_t kF1MK2_buttonsDataSize = 5;
  static constexpr uint8_t kF1MK2_nPotentiometers = 8;

  void init() override;
  bool sendLedsAndDisplay();
  bool read();
  
  void processButtons( const Transfer& );
  
  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led getLed(Device::Button) const noexcept;
  Led getLed(Device::Pad) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;
  
  GDisplayDummy               m_displayDummy;
  LCDDisplay7Segments         m_lcdDisplay;

  tRawData                    m_leds;

  tRawData                    m_buttons;
  bool                        m_buttonStates[kF1MK2_nButtons];
  uint16_t                    m_potentiometersValues[kF1MK2_nPotentiometers];
  uint8_t                     m_encoderValue;

  bool                        m_isDirtyLeds;

};
  
//--------------------------------------------------------------------------------------------------
  
} // kio
} // sl
