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
  typedef enum{
  
    
    /* NOT_ASSIGNED = 59 */
    /* NOT_ASSIGNED = 60 */
    /* NOT_ASSIGNED = 61 */
/*
  outputBuffer[16] = 0x00; // Mute
  outputBuffer[17] = 0x00; // Solo
  outputBuffer[18] = 0x00; // Select
  outputBuffer[19] = 0x00; // Duplicate
  outputBuffer[20] = 0x00; // Navigate
  outputBuffer[21] = 0x00; // Keyboard
  outputBuffer[22] = 0x00; // Pattern
  outputBuffer[23] = 0x00; // Scene
  
  outputBuffer[24] = 0x00; // Shift
  outputBuffer[25] = 0x00; // Erase
  outputBuffer[26] = 0x00; // Grid
  outputBuffer[27] = 0x00; // >
  outputBuffer[28] = 0x00; // REC
  outputBuffer[29] = 0x00; // Play
  outputBuffer[30] = 0xff; // n/a
  outputBuffer[31] = 0x00; // <
  outputBuffer[32] = 0x00; // Loop
  outputBuffer[33] = 0x00; // Group H
  outputBuffer[34] = 0x00; // Group G
  outputBuffer[35] = 0x00; // Group D
  outputBuffer[36] = 0x00; // Group C
  outputBuffer[37] = 0x00; // Group F
  outputBuffer[38] = 0x00; // Group E
  outputBuffer[39] = 0x00; // Group B
  outputBuffer[40] = 0x00; // Group A
  outputBuffer[41] = 0x00; // F2
  outputBuffer[42] = 0x00; // F1
  outputBuffer[43] = 0x00; // >
  outputBuffer[44] = 0x00; // <
  outputBuffer[45] = 0x00; // SAMPLING
  outputBuffer[46] = 0x00; // Browse
  outputBuffer[47] = 0x00; // Step
  outputBuffer[48] = 0x00; // Control
  
  outputBuffer[49] = 0x00; // B8
  outputBuffer[50] = 0x00; // B7
  outputBuffer[51] = 0x00; // B6
  outputBuffer[52] = 0x00; // B5
  outputBuffer[53] = 0x00; // B4
  outputBuffer[54] = 0x00; // B3
  outputBuffer[55] = 0x00; // B2
  outputBuffer[56] = 0x00; // B1
  outputBuffer[57] = 0x00; // Note repeat
  outputBuffer[58] = 0x5C; // Display backlight
  outputBuffer[59] = 0x00; // n/a
  outputBuffer[60] = 0x00; // n/a
  outputBuffer[61] = 0x00; // n/a
  */
  } tMK1Leds;
  
public:
  
  DeviceMaschineMK1();
  ~DeviceMaschineMK1() override;
  
  bool connect() override;
  
  void setLed( Device::Button, uint8_t                   ) override;
  void setLed( Device::Button, uint8_t, uint8_t, uint8_t ) override;
  
  GDisplay* getDisplay( uint8_t displayIndex_ );
  bool tick() override;

private:

  enum class Led : uint8_t;
  
  static constexpr uint8_t kMASMK1_nDisplays      = 2;
  static constexpr uint8_t kMASMK1_ledsDataSize   = 62;

  void init() override;

  void initDisplay( uint8_t displayIndex_ );
  bool sendFrame( uint8_t displayIndex_ );
  bool sendLeds();
  bool read();
  
  Led getLed( Device::Button ) const noexcept;
  
  tPtr<GDisplay>      m_displays[kMASMK1_nDisplays];
  tRawData            m_leds;
  
  bool                m_isDirtyLedGroup0;
  bool                m_isDirtyLedGroup1;
  
  uint8_t             m_pendingAcks;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
