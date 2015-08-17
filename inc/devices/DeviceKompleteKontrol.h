/*----------------------------------------------------------------------------------------------------------------------   

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include "Device.h"
#include "gfx/GDisplay.h"
 
namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

class DeviceKompleteKontrol : public Device
{
 
public:
  
  DeviceKompleteKontrol(tPtr<DeviceHandle>, uint8_t numKeys_);
  ~DeviceKompleteKontrol() override;
  
  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Key, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;
  
  GDisplay* getDisplay( uint8_t displayIndex_ ) override;
  bool tick() override;

private:

  enum class Led    : uint16_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kKK_nButtons = 37;
  static constexpr uint8_t kKK_buttonsDataSize = 5;
  static constexpr uint8_t kKK_ledsDataSize = 25;
  

  void init() override {}
  bool sendLeds();
  bool read();
  
  void processButtons( const Transfer& );
  
  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led);
  Led getLed(Device::Key) const noexcept;
  Led getLed(Device::Button) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;

  tRawData            m_leds;
  tRawData            m_ledsKeys;
  tRawData            m_buttons;
  bool                m_buttonStates[kKK_nButtons];
  uint8_t             m_encoderValue;

  uint8_t             m_numKeys;
    
  bool                m_isDirtyLeds;
  bool                m_isDirtyKeyLeds;

};
  
//----------------------------------------------------------------------------------------------------------------------
  
} // kio
} // sl
