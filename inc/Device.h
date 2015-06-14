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

#include "comm/DeviceHandle.h"

namespace sl
{
namespace kio
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
    Snap = All,
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

  enum class Encoder : uint8_t
  {
    Main,
    Volume = Main,
    Tempo,
    Swing,
    Encoder1,
    Encoder2,
    Encoder3,
    Encoder4,
    Encoder5,
    Encoder6,
    Encoder7,
    Encoder8,
    Unknown,
  };

  enum class Pad : uint8_t
  {
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
    Unknown,
  };

  using tCbButtonChanged = std::function<void(Button button_, bool buttonState_, bool shiftPressed)>;
  using tCbEncoderChanged = std::function<void(Encoder encoder_, bool valueIncreased_, bool shiftPressed_)>;
  using tCbPadChanged = std::function<void(Pad pad_, uint16_t value_, bool shiftPressed)>;
  
  enum class Type
  {
    Unknown,
    MaschineMk1,
    MaschineMk2,
    MaschineMikroMk1,
    MaschineMikroMk2,
  };
  
  static Type getType(uint16_t, uint16_t);
  
  Device(tPtr<DeviceHandle> pDeviceHandle_)
    : m_pDeviceHandle(std::move(pDeviceHandle_))
  {
  }

  virtual ~Device()
  {
  }

//  virtual bool connect() = 0;

  virtual void init() = 0;

  virtual bool tick() = 0;
  virtual GDisplay* getDisplay(uint8_t displayIndex_) = 0;

  virtual void setLed(Button, uint8_t) = 0;
  virtual void setLed(Button, uint8_t, uint8_t, uint8_t) = 0;

  void setCallbackButtonChanged(tCbButtonChanged cbButtonChanged_){ m_cbButtonChanged = cbButtonChanged_; }
  void setCallbackEncoderChanged(tCbEncoderChanged cbEncoderChanged_){ m_cbEncoderChanged = cbEncoderChanged_; }
  void setCallbackPadChanged(tCbPadChanged cbPadChanged_){ m_cbPadChanged = cbPadChanged_;}

  DeviceHandle* getDeviceHandle(){ return m_pDeviceHandle.get(); }
  
protected:
  void buttonChanged(Button button_, bool buttonState_, bool shiftPressed_)
  {
    if(m_cbButtonChanged)
    {
      m_cbButtonChanged(button_,buttonState_,shiftPressed_);
    }
  }
  
  void encoderChanged(Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
  {
    if(m_cbEncoderChanged)
    {
      m_cbEncoderChanged(encoder_,valueIncreased_,shiftPressed_);
    }
  }
  
  void padChanged(Pad pad_, uint16_t value_, bool shiftPressed_)
  {
    if(m_cbPadChanged)
    {
      m_cbPadChanged(pad_,value_,shiftPressed_);
    }
  }
  
private:

  tCbButtonChanged    m_cbButtonChanged;
  tCbEncoderChanged   m_cbEncoderChanged;
  tCbPadChanged       m_cbPadChanged;
  
  tPtr<DeviceHandle>  m_pDeviceHandle;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
