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
#include "comm/Transfer.h"

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

class DeviceMaschineMK1 : public Device
{
   
public:
  
  DeviceMaschineMK1(tPtr<DeviceHandle>);
  ~DeviceMaschineMK1() override;
  
  void setLed( Device::Button, uint8_t                   ) override;
  void setLed( Device::Button, uint8_t, uint8_t, uint8_t ) override;
  
  GDisplay* getDisplay( uint8_t displayIndex_ );
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

  using tBuffer = util::CircularBuffer<uint16_t, kMASMK1_padsBufferSize>;

  void init() override;

  void initDisplay( uint8_t displayIndex_ );
  bool sendFrame( uint8_t displayIndex_ );
  bool sendLeds();
  bool read();
  
  void processPads(const Transfer&);
  void processButtons(const Transfer&);
  void processEncoders(const Transfer&);

  Led getLed( Device::Button ) const noexcept;
  Device::Button getDeviceButton(Button btn_) const noexcept;
  Device::Encoder getDeviceEncoder(Encoder btn_) const noexcept;
  bool isButtonPressed(Button button) const noexcept;
  bool isButtonPressed(const Transfer&, Button button_) const noexcept;

  tPtr<GDisplay>      m_displays[kMASMK1_nDisplays];
  tRawData            m_leds;
  tRawData            m_buttons;

  bool                m_buttonStates[kMASMK1_nButtons];
  uint16_t            m_encoderValues[kMASMK1_nEncoders];

  tBuffer             m_padsRawData[kMASMK1_nPads];
  uint16_t            m_padsAvgData[kMASMK1_nPads];

  bool                m_isDirtyLedGroup0;
  bool                m_isDirtyLedGroup1;
  bool                m_encodersInitialized;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
