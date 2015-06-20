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

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class DeviceMaschineMK2 : public Device
{
 
public:
  
  DeviceMaschineMK2(tPtr<DeviceHandle>);
  ~DeviceMaschineMK2() override;
  
  void setLed( Device::Button, uint8_t                   ) override;
  void setLed( Device::Button, uint8_t, uint8_t, uint8_t ) override;
  
  GDisplay* getDisplay( uint8_t displayIndex_ ) override;
  bool tick() override;

private:

  enum class Led    : uint8_t;
  enum class Button : uint8_t;
 
  static constexpr uint8_t kMASMK2_nDisplays         = 2;
  static constexpr uint8_t kMASMK2_nButtons          = 45;
  static constexpr uint8_t kMASMK2_ledsDataSize      = 56;
  static constexpr uint8_t kMASMK2_buttonsDataSize   = 5;
  static constexpr uint8_t kMASMK2_padDataSize       = 64;
  static constexpr uint8_t kMASMK2_nPads             = 16;
  static constexpr uint8_t kMASMK2_padsBufferSize    = 16;

  using tBuffer = util::CircularBuffer<uint16_t, kMASMK2_padsBufferSize>;

  void init() override;

  void initDisplay() const;
  bool sendFrame(uint8_t displayIndex);
  bool sendLeds();
  bool read();
  
  void processButtons( const Transfer& );
  void processPads( const Transfer& );
  
  bool isRGBLed( Led );
  Led getLed( Device::Button ) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;

  tPtr<GDisplay>      m_displays[kMASMK2_nDisplays];
  
  tRawData            m_ledsButtons;
  tRawData            m_ledsGroups;
  tRawData            m_ledsPads;

  tRawData            m_buttons;
  bool                m_buttonStates[kMASMK2_nButtons];
  uint8_t             m_encoderValue;
  
  tBuffer             m_padsRawData[ kMASMK2_nPads ];
  uint16_t            m_padsAvgData[ kMASMK2_nPads ];
  
  bool                m_isDirtyLeds;

};

//----------------------------------------------------------------------------------------------------------------------
  
} // kio
} // sl
