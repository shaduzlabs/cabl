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

#ifndef SL_DEVICE_MIKRO_MK2_H
#define SL_DEVICE_MIKRO_MK2_H

#include "Device.h"
#include "gfx/GDisplay.h"
/*
 
 uint8_t buf[79];
 for(int i=0;i<79;i++)
 buf[i] = 0x00;
 buf[0] = 0x80;
 
 int idx = 1;
 
 buf[idx++] = 0xff; // F1
 buf[idx++] = 0xff; // F2
 buf[idx++] = 0xff; // F3
 buf[idx++] = 0xff; // CONTROL
 
 buf[idx++] = 0xff; // NAV
 buf[idx++] = 0xff; // <
 buf[idx++] = 0xff; // >
 buf[idx++] = 0xff; // MAIN
 
 buf[idx++] = 0xff; // Group (R)
 buf[idx++] = 0x00; // Group (G)
 buf[idx++] = 0x00; // Group (B)
 
 buf[idx++] = 0xff; // Browse
 buf[idx++] = 0xff; // Sampling
 buf[idx++] = 0xff; // Note repeat
 
 buf[idx++] = 0xff; // Restart
 buf[idx++] = 0xff; // <
 buf[idx++] = 0xff; // >
 buf[idx++] = 0xff; // Grid
 
 buf[idx++] = 0xff; // Play
 buf[idx++] = 0xff; // REC
 buf[idx++] = 0xff; // Erase
 buf[idx++] = 0xff; // SHIFT
 
 buf[idx++] = 0xff; // Scene
 buf[idx++] = 0xff; // Pattern
 buf[idx++] = 0xff; // Pad mode
 buf[idx++] = 0xff; // View
 
 buf[idx++] = 0xff; // Duplicate
 buf[idx++] = 0xff; // Select
 buf[idx++] = 0xff; // Solo
 buf[idx++] = 0xff; // Mute
 
 buf[idx++] = 0xff; // PAD 13 (R)
 buf[idx++] = 0xff; // PAD 13 (G)
 buf[idx++] = 0xff; // PAD 13 (B)
 
 buf[idx++] = 0xff; // PAD 14 (R)
 buf[idx++] = 0xff; // PAD 14 (G)
 buf[idx++] = 0xff; // PAD 14 (B)
 
 buf[idx++] = 0xff; // PAD 15 (R)
 buf[idx++] = 0xff; // PAD 15 (G)
 buf[idx++] = 0xff; // PAD 15 (B)
 
 buf[idx++] = 0xff; // PAD 16 (R)
 buf[idx++] = 0xff; // PAD 16 (G)
 buf[idx++] = 0xff; // PAD 16 (B)
 
 buf[idx++] = 0xff; // PAD 9  (R)
 buf[idx++] = 0xff; // PAD 9  (G)
 buf[idx++] = 0xff; // PAD 9  (B)
 
 buf[idx++] = 0xff; // PAD 10 (R)
 buf[idx++] = 0xff; // PAD 10 (G)
 buf[idx++] = 0xff; // PAD 10 (B)
 
 buf[idx++] = 0xff; // PAD 11 (R)
 buf[idx++] = 0xff; // PAD 11 (G)
 buf[idx++] = 0xff; // PAD 11 (B)
 
 buf[idx++] = 0xff; // PAD 12 (R)
 buf[idx++] = 0xff; // PAD 12 (G)
 buf[idx++] = 0xff; // PAD 12 (B)
 
 buf[idx++] = 0xff; // PAD 5  (R)
 buf[idx++] = 0xff; // PAD 5  (G)
 buf[idx++] = 0xff; // PAD 5  (B)
 
 buf[idx++] = 0xff; // PAD 6  (R)
 buf[idx++] = 0xff; // PAD 6  (G)
 buf[idx++] = 0xff; // PAD 6  (B)
 
 buf[idx++] = 0xff; // PAD 7  (R)
 buf[idx++] = 0xff; // PAD 7  (G)
 buf[idx++] = 0xff; // PAD 7  (B)
 
 buf[idx++] = 0xff; // PAD 8  (R)
 buf[idx++] = 0xff; // PAD 8  (G)
 buf[idx++] = 0xff; // PAD 8  (B)
 
 buf[idx++] = 0xff; // PAD 1  (R)
 buf[idx++] = 0xff; // PAD 1  (G)
 buf[idx++] = 0xff; // PAD 1  (B)
 
 buf[idx++] = 0xff; // PAD 2  (R)
 buf[idx++] = 0xff; // PAD 2  (G)
 buf[idx++] = 0xff; // PAD 2  (B)
 
 buf[idx++] = 0xff; // PAD 3  (R)
 buf[idx++] = 0xff; // PAD 3  (G)
 buf[idx++] = 0xff; // PAD 3  (B)
 
 buf[idx++] = 0xff; // PAD 4  (R)
 buf[idx++] = 0xff; // PAD 4  (G)
 buf[idx++] = 0xff; // PAD 4  (B)
 
 //  Transfer transfer( buf, 79, 0x01 );*/

// -----------------------------------------
/*
 transfer = driver.read();
 uint8_t myChar = transfer.getData()[0];
 
 while( myChar == 0x20 )
 {
 transfer = driver.read();
 myChar = transfer.getData()[0];
 }
 
 for (int i = 0; i < transfer.getSize(); i++)
 {
 printf( "%02hhx ", transfer.getData()[i] );
 }
 */
/*
 
 1st byte: 0x01
 
 2nd byte: 0x01 - SHIFT
 0x02 - ERASE
 0x04 - REC
 0x08 - PLAY
 0x10 - GRID
 0x20 - >
 0x40 - <
 0x80 - RESTART
 
 3rd byte: 0x01 - N/A
 0x02 - N/A
 0x04 - N/A
 0x08 - ENCODER PUSHBUTTON
 0x10 - NOTE REPEAT
 0x20 - SAMPLING
 0x40 - BROWSE
 0x80 - GROUP
 
 4th byte: 0x01 - MAIN
 0x02 - >
 0x04 - <
 0x08 - NAV
 0x10 - CONTROL
 0x20 - F3
 0x40 - F2
 0x80 - F1
 
 5th byte: 0x01 - MUTE
 0x02 - SOLO
 0x04 - SELECT
 0x08 - DUPLICATE
 0x10 - VIEW
 0x20 - PAD MODE
 0x40 - PATTERN
 0x80 - SCENE
 
 6th byte: Encoder value (0x0 to 0x0e)
 */
namespace sl
{
 
class DeviceMaschineMikroMK2 : public Device
{
 
public:
  
  DeviceMaschineMikroMK2();
  ~DeviceMaschineMikroMK2() override;
  
  bool connect() override;

  void setLed( Device::Button, uint8_t                   ) override;
  void setLed( Device::Button, uint8_t, uint8_t, uint8_t ) override;
  
  GDisplay* getDisplay( uint8_t displayIndex_ ) override;
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
  
  bool isRGBLed( Led );
  Led getLed( Device::Button ) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;

  tPtr<GDisplay>      m_display;
  
  tRawData            m_leds;
  tRawData            m_buttons;
  bool                m_buttonStates[kMikroMK2_nButtons];
  uint8_t             m_encoderValue;
  
  tBuffer             m_padsRawData[ kMikroMK2_nPads ];
  uint16_t            m_padsAvgData[ kMikroMK2_nPads ];
  
  bool                m_isDirtyLeds;

};
  
//----------------------------------------------------------------------------------------------------------------------
  
} // sl

#endif // SL_DEVICE_MIKRO_MK2_H
