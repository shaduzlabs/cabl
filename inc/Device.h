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

// STL includes
#include <functional>

#include "comm/Driver.h"

namespace sl
{

class GDisplay;

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

  using tCbButtonChanged = std::function<void(Button button_, bool buttonState_, bool shiftPressed)>;
  using tCbEncoderChanged = std::function<void(uint8_t encoderIndex_, bool valueIncreased_, bool shiftPressed_)>;
  
  Device(Driver::tDriver tDriver_)
    : m_driver(tDriver_)
  {
  }

  virtual ~Device()
  {
  }

  virtual bool connect() = 0;
  virtual void init() = 0;

  virtual void tick() = 0;
  virtual GDisplay* getDisplay(uint8_t displayIndex_) = 0;


  virtual void setLed(Button, uint8_t) = 0;
  virtual void setLed(Button, uint8_t, uint8_t, uint8_t) = 0;
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
  
  void setCallbackButtonChanged(tCbButtonChanged cbButtonChanged_){ m_cbButtonChanged = cbButtonChanged_; }
  void setCallbackEncoderChanged(tCbEncoderChanged cbEncoderChanged_){ m_cbEncoderChanged = cbEncoderChanged_; }

  void setCallbackPadsChanged(void (*fptr)(uint16_t mask, const uint16_t* pPads))
  {
    m_padsChangeCallback = fptr;
  }

protected:
  void (*m_padsChangeCallback)(uint16_t mask, const uint16_t* pPads);

protected:

  Driver& getDriver(){ return m_driver; }
  
  void buttonChanged(Button button_, bool buttonState_, bool shiftPressed_)
  {
    if(m_cbButtonChanged)
    {
      m_cbButtonChanged(button_,buttonState_,shiftPressed_);
    }
  }
  
  void encoderChanged(uint8_t encoderIndex_, bool valueIncreased_, bool shiftPressed_)
  {
    if(m_cbEncoderChanged)
    {
      m_cbEncoderChanged(encoderIndex_,valueIncreased_,shiftPressed_);
    }
  }
private:

  tCbButtonChanged    m_cbButtonChanged;
  tCbEncoderChanged   m_cbEncoderChanged;
  
  Driver m_driver;
};

//----------------------------------------------------------------------------------------------------------------------

} // sl
