/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

// STL includes
#include <functional>
#include <mutex>

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
#include <RtMidi.h>
#endif

//--------------------------------------------------------------------------------------------------

#include "devices/DeviceRegistrar.h"
#include "comm/DeviceHandle.h"
#include "comm/DeviceDescriptor.h"
#include "util/LedColor.h"
#include "gfx/DrawingContext.h"

namespace sl
{
namespace cabl
{

class GDisplay;
class LCDDisplay;

namespace devices
{

class Device
{

public:

// clang-format off

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
    MainEncoder,
    Scale, Scales = Scale,
    Arp,
    Rwd,
    Ffw,
    Stop,
    PageLeft,
    PageRight,
    PresetUp,
    Instance,
    PresetDown,
    Back,
    NavigateUp,
    NavigateLeft,
    NavigateDown,
    NavigateRight,
    OctaveUp,
    OctaveDown,
    TouchEncoder1,
    TouchEncoder2,
    TouchEncoder3,
    TouchEncoder4,
    TouchEncoder5,
    TouchEncoder6,
    TouchEncoder7,
    TouchEncoder8,
    TouchEncoder9,
    TouchEncoderMain,
    TouchEncoderMain2,

    // Traktor F1
    Size,
    Type,
    Reverse,
    Capture,
    Quant, Quantize = Quant,
    Sync,
    Pad1, Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
    Pad9, Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16,
    Stop1, Stop2, Stop3, Stop4,

    // Push
    Btn1Row1,
    Btn2Row1,
    Btn3Row1,
    Btn4Row1,
    Btn5Row1,
    Btn6Row1,
    Btn7Row1,
    Btn8Row1,
    Btn1Row2,
    Btn2Row2,
    Btn3Row2,
    Btn4Row2,
    Btn5Row2,
    Btn6Row2,
    Btn7Row2,
    Btn8Row2,
    Grid1_4,
    Grid1_4T,
    Grid1_8,
    Grid1_8T,
    Grid1_16,
    Grid1_16T,
    Grid1_32,
    Grid1_32T,
    TapTempo,
    Metronome,
    TouchStripTap,
    Master,
    Setup,
    Layout,
    Convert,
    Note,
    Session,
    AddEffect,
    AddTrack,
    Repeat,
    Accent,
    User,
    In,
    Out,
    New,
    Automation,
    FixedLength,
    Device,
    Track,
    Clip,
    PanSend,
    Double,
    Delete,
    Undo,

    Unknown,
  };

  enum class Encoder : uint8_t
  {
    Main,
    Volume = Main,
    Tempo,
    Main2 = Tempo,
    Swing,
    Encoder1,
    Encoder2,
    Encoder3,
    Encoder4,
    Encoder5,
    Encoder6,
    Encoder7,
    Encoder8,
    Encoder9,
    Unknown,
  };

  enum class Pad : uint8_t
  {
    Pad1,   Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
    Pad9,  Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16,
    Pad17, Pad18, Pad19, Pad20, Pad21, Pad22, Pad23, Pad24,
    Pad25, Pad26, Pad27, Pad28, Pad29, Pad30, Pad31, Pad32,
    Pad33, Pad34, Pad35, Pad36, Pad37, Pad38, Pad39, Pad40,
    Pad41, Pad42, Pad43, Pad44, Pad45, Pad46, Pad47, Pad48,
    Pad49, Pad50, Pad51, Pad52, Pad53, Pad54, Pad55, Pad56,
    Pad57, Pad58, Pad59, Pad60, Pad61, Pad62, Pad63, Pad64,
    Unknown,
  };


  enum class Key : uint8_t
  {
    Key1,   Key2,   Key3,   Key4,   Key5,   Key6,   Key7,   Key8,   Key9,   Key10,  Key11,  Key12,
    Key13,  Key14,  Key15,  Key16,  Key17,  Key18,  Key19,  Key20,  Key21,  Key22,  Key23,  Key24,
    Key25,  Key26,  Key27,  Key28,  Key29,  Key30,  Key31,  Key32,  Key33,  Key34,  Key35,  Key36,
    Key37,  Key38,  Key39,  Key40,  Key41,  Key42,  Key43,  Key44,  Key45,  Key46,  Key47,  Key48,
    Key49,  Key50,  Key51,  Key52,  Key53,  Key54,  Key55,  Key56,  Key57,  Key58,  Key59,  Key60,
    Key61,  Key62,  Key63,  Key64,  Key65,  Key66,  Key67,  Key68,  Key69,  Key70,  Key71,  Key72,
    Key73,  Key74,  Key75,  Key76,  Key77,  Key78,  Key79,  Key80,  Key81,  Key82,  Key83,  Key84,
    Key85,  Key86,  Key87,  Key88,  Key89,  Key90,  Key91,  Key92,  Key93,  Key94,  Key95,  Key96,
    Key97,  Key98,  Key99,  Key100, Key101, Key102, Key103, Key104, Key105, Key106, Key107, Key108,
    Key109, Key110, Key111, Key112, Key113, Key114, Key115, Key116, Key117, Key118, Key119, Key120,
    Key121, Key122, Key123, Key124, Key125, Key126, Key127, Key128,
    Unknown,
  };

  enum class Potentiometer : uint8_t
  {
    CenterDetented1, CenterDetented2, CenterDetented3, CenterDetented4,
    CenterDetented5, CenterDetented6, CenterDetented7, CenterDetented8,
    Fader1,  Fader2,  Fader3,  Fader4,  Fader5,  Fader6,  Fader7,  Fader8,
    Fader9,  Fader10, Fader11, Fader12, Fader13, Fader14, Fader15, Fader16,
    Unknown,
  };

// clang-format on

  using tCbRender = std::function<void(void)>;
  using tCbDisconnect = std::function<void(void)>;

  using tCbButtonChanged = std::function<void(Button btn_, bool state_, bool shiftKey_)>;
  using tCbEncoderChanged = std::function<void(Encoder enc_, bool valIncreased_, bool shiftKey_)>;
  using tCbPadChanged = std::function<void(Pad pad_, uint16_t val_, bool shiftKey_)>;
  using tCbKeyChanged = std::function<void(Key pad_, uint16_t val_, bool shiftKey_)>;
  using tCbPotentiometerChanged
    = std::function<void(Potentiometer pad_, uint16_t val_, bool shiftKey_)>;

  enum class Type
  {
    Unknown,
    MaschineMk1,
    MaschineMk2,
    MaschineMikroMk1,
    MaschineMikroMk2,
  };

  Device() = default;
  virtual ~Device() = default;

  void setDeviceHandle(tPtr<DeviceHandle> pDeviceHandle_)
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    m_pDeviceHandle  = std::move(pDeviceHandle_);
  }
  
  void resetDeviceHandle()
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    m_pDeviceHandle  = nullptr;
  }

  virtual void init() = 0;

  virtual GDisplay* displayGraphic(size_t displayIndex_) = 0;
  virtual LCDDisplay* displayLCD(size_t displayIndex_) = 0;
  virtual size_t numOfGraphicDisplays() { return 0; }
  virtual size_t numOfLCDDisplays() { return 0; }

  virtual DrawingContext& drawingContext(size_t contextIndex_)
  {
    static DrawingContext s_dummyContext{0, 0, 0};
    return s_dummyContext;
  }

  virtual void setLed(Button, const util::LedColor&) = 0;

  virtual void setLed(Pad, const util::LedColor&)
  {
  }
  virtual void setLed(Key, const util::LedColor&)
  {
  }

  virtual void sendMidiMsg(tRawData) = 0;

  void setCallbackDisconnect(tCbDisconnect cbDisconnect_)
  {
    m_cbDisconnect = cbDisconnect_;
  }

  void setCallbackRender(tCbRender cbRender_)
  {
    m_cbRender = cbRender_;
  }
  
  void setCallbackButtonChanged(tCbButtonChanged cbButtonChanged_)
  {
    m_cbButtonChanged = cbButtonChanged_;
  }

  void setCallbackEncoderChanged(tCbEncoderChanged cbEncoderChanged_)
  {
    m_cbEncoderChanged = cbEncoderChanged_;
  }

  void setCallbackPadChanged(tCbPadChanged cbPadChanged_)
  {
    m_cbPadChanged = cbPadChanged_;
  }

  void setCallbackKeyChanged(tCbKeyChanged cbKeyChanged_)
  {
    m_cbKeyChanged = cbKeyChanged_;
  }

  void setCallbackPotentiometerChanged(tCbPotentiometerChanged cbPotentiometerChanged_)
  {
    m_cbPotentiometerChanged = cbPotentiometerChanged_;
  }

  bool hasDeviceHandle()
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    return static_cast<bool>(m_pDeviceHandle);
  }
  
protected:

  virtual bool tick() = 0;

  bool writeToDeviceHandle(const Transfer& transfer_, uint8_t endpoint_ )
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    
    if(m_pDeviceHandle)
    {
      return m_pDeviceHandle->write(transfer_, endpoint_);
    }
    
    return false;
  }

  bool readFromDeviceHandle(Transfer& transfer_, uint8_t endpoint_)
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    if(m_pDeviceHandle)
    {
      return m_pDeviceHandle->read(transfer_, endpoint_);
    }
    
    return false;
  }
  
  void readFromDeviceHandleAsync(uint8_t endpoint_, DeviceHandle::tCbRead cbRead_)
  {
    std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
    if(m_pDeviceHandle)
    {
      return m_pDeviceHandle->readAsync(endpoint_, cbRead_);
    }
  }
  

  void buttonChanged(Button button_, bool buttonState_, bool shiftPressed_)
  {
    if (m_cbButtonChanged)
    {
      m_cbButtonChanged(button_, buttonState_, shiftPressed_);
    }
  }

  void encoderChanged(Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
  {
    if (m_cbEncoderChanged)
    {
      m_cbEncoderChanged(encoder_, valueIncreased_, shiftPressed_);
    }
  }

  void padChanged(Pad pad_, uint16_t value_, bool shiftPressed_)
  {
    if (m_cbPadChanged)
    {
      m_cbPadChanged(pad_, value_, shiftPressed_);
    }
  }

  void keyChanged(Key key_, uint16_t value_, bool shiftPressed_)
  {
    if (m_cbKeyChanged)
    {
      m_cbKeyChanged(key_, value_, shiftPressed_);
    }
  }

  void potentiometerChanged(Potentiometer potentiometer_, uint16_t value_, bool shiftPressed_)
  {
    if (m_cbPotentiometerChanged)
    {
      m_cbPotentiometerChanged(potentiometer_, value_, shiftPressed_);
    }
  }

private:
  bool onTick()
  {
    if(!hasDeviceHandle())
    {
      return false;
    }
    
    render();
    return tick();
  }

  void onConnect()
  {
    init();
  }

  void onDisconnect()
  {
    resetDeviceHandle();
    if (m_cbDisconnect)
    {
      m_cbDisconnect();
    }
  }
  
  void render()
  {
    if (m_cbRender)
    {
      m_cbRender();
    }
  }

  tCbDisconnect    m_cbDisconnect;
  tCbRender        m_cbRender;

  tCbButtonChanged m_cbButtonChanged;
  tCbEncoderChanged m_cbEncoderChanged;
  tCbPadChanged m_cbPadChanged;
  tCbKeyChanged m_cbKeyChanged;
  tCbPotentiometerChanged m_cbPotentiometerChanged;

  std::mutex         m_mtxDeviceHandle;
  tPtr<DeviceHandle> m_pDeviceHandle;

  friend class Coordinator;
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
