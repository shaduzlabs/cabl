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
  
  typedef enum{
     btnLedControl = 1,
     btnLedStep,
     btnLedBrowse,
     btnLedSampling,
     brnLedBrowseLeft,
     btnLedBrowseRight,
     btnLedAll,
     btnLedAutoWrite,
  
     btnLedP1,
     btnLedP2,
     btnLedP3,
     btnLedP4,
     btnLedP5,
     btnLedP6,
     btnLedP7,
     btnLedP8,

     btnLedScene,
     btnLedPattern,
     btnLedPadMode,
     btnLedNavigate,
     btnLedDuplicate,
     btnLedSelect,
     btnLedSolo,
     btnLedMute,
  
     btnLedVolume,
     btnLedSwing,
     btnLedTempo,
     btnLedMasterLeft,
     btnLedMasterRight,
     btnLedEnter,
     btnLedNoteRepeat,

     btnLedRestart = 49,
     btnLedStepLeft,
     btnLedStepRight,
     btnLedGrid,
     btnLedPlay,
     btnLedRec,
     btnLedErase,
     btnLedShift,
    
  } eButtonLed;
  
   typedef enum{
     
     btnP1,
     btnP2,
     btnP3,
     btnP4,
     btnP5,
     btnP6,
     btnP7,
     btnP8,
    
     btnControl,
     btnStep,
     btnBrowse,
     btnSampling,
     brnBrowseLeft,
     btnBrowseRight,
     btnAll,
     btnAutoWrite,
    
     btnVolume,
     btnSwing,
     btnTempo,
     btnMasterLeft,
     btnMasterRight,
     btnEnter,
     btnNoteRepeat,
     btnNotUsed,

     btnGroupA,
     btnGroupB,
     btnGroupC,
     btnGroupD,
     btnGroupE,
     btnGroupF,
     btnGroupG,
     btnGroupH,
  
     btnRestart,
     btnStepLeft,
     btnStepRight,
     btnGrid,
     btnPlay,
     btnRec,
     btnErase,
     btnShift,
     
     btnScene,
     btnPattern,
     btnPadMode,
     btnNavigate,
     btnDuplicate,
     btnSelect,
     btnSolo,
     btnMute,

     btnLAST,
  } eButton;

  DeviceMaschineMK2();
  ~DeviceMaschineMK2() override;
  
  void setLed( Device::Led, uint8_t                   ) override;
  void setLed( Device::Led, uint8_t, uint8_t, uint8_t ) override;
  
};

//----------------------------------------------------------------------------------------------------------------------
  
} // kio
} // sl
