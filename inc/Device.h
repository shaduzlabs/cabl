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

#include "comm/Driver.h"

namespace sl
{

class Device
{
  
public:
  
  enum class Button : uint8_t
  {
    Control,
    Step,
    Browse,
    Sampling,
    BrowseLeft,
    BrowseRight,
    AutoWrite,
    F2 = AutoWrite,
    All,
    F1 = All,
    DisplayButton1,
    DisplayButton2,
    DisplayButton3,
    DisplayButton4,
    DisplayButton5,
    DisplayButton6,
    DisplayButton7,
    DisplayButton8, 
    Scene,
    Pattern,
    PadMode,
    Keyboard = PadMode,
    View,
    Navigate = View,
    Duplicate,
    Select,
    Solo,
    Mute,
    Volume,
    Swing,
    Tempo,
    MasterLeft,
    MasterRight,
    Enter,
    Tap = Enter,
    NoteRepeat,
    Group,
    GroupA,
    GroupB,
    GroupC,
    GroupD,
    GroupE,
    GroupF,
    GroupG,
    GroupH,
    Restart,
    Loop = Restart,
    TransportLeft,
    TransportRight,
    Grid,
    Play,
    Rec,
    Erase,
    Shift,
    F3,
    Nav,
    Main, // Enter
    Pad1,
    Pad2,
    Pad3,
    Pad4,
    Pad5,
    Pad6,
    Pad7,
    Pad8,
    Pad9,
    Pad10,
    Pad11,
    Pad12,
    Pad13,
    Pad14,
    Pad15,
    Pad16,
    MainEncoder,
    Unknown,
  };
  
  Device( Driver::tDriver tDriver_ )
  : m_buttonsChangeCallback( nullptr )
  , m_encodersChangeCallback( nullptr )
  , m_padsChangeCallback( nullptr )
  , m_driver( tDriver_ )
  {
  }
  
  virtual ~Device(){
  
  }
  
  virtual bool connect() = 0;
  virtual void init() = 0;
  
  virtual void setLed( Button, uint8_t                   ) = 0;
  virtual void setLed( Button, uint8_t, uint8_t, uint8_t ) = 0;
  /*
  virtual uint8_t getNumberOfDisplays() const;
  virtual bool getNumberOfRGBLeds() const;
  virtual bool getNumberOfLeds() const;

  virtual bool hasRGBLeds() const;
  virtual bool hasStandardDisplays() const;


  virtual bool setCallbackButtons() const;
  virtual bool setCallbackDials() const;
  virtual bool setCallbackPads() const;
  */
  
  void setCallbackButtonsPressed(void (*fptr)(uint32_t mask1, uint32_t mask2, const bool* pButtons))
    { m_buttonsChangeCallback = fptr; }
  
  void setCallbackEncodersChanged(void (*fptr)(uint16_t mask, const uint16_t* pEncoders))
    { m_encodersChangeCallback = fptr; }
  
  void setCallbackPadsChanged(void (*fptr)(uint16_t mask, const uint16_t* pPads))
    { m_padsChangeCallback = fptr; }
  
protected:

  void (*m_buttonsChangeCallback  )( uint32_t mask1, uint32_t mask2, const bool* pButtons );
  void (*m_encodersChangeCallback )( uint16_t mask, const uint16_t* pEncoders );
  void (*m_padsChangeCallback     )( uint16_t mask, const uint16_t* pPads );

  Driver                m_driver;

};

//----------------------------------------------------------------------------------------------------------------------

} // sl
