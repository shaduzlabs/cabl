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

#include "cabl/comm/DeviceDescriptor.h"
#include "cabl/comm/DeviceHandle.h"
#include "cabl/devices/DeviceRegistrar.h"

#include "cabl/util/Color.h"

namespace sl
{
namespace cabl
{

class Canvas;
class TextDisplay;
class LedArray;

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
    Stop1, Stop2, Stop3, Stop4,

    Pad1,  Pad2,  Pad3,  Pad4,  Pad5,  Pad6,  Pad7,  Pad8,
    Pad9,  Pad10, Pad11, Pad12, Pad13, Pad14, Pad15, Pad16,
    Pad17, Pad18, Pad19, Pad20, Pad21, Pad22, Pad23, Pad24,
    Pad25, Pad26, Pad27, Pad28, Pad29, Pad30, Pad31, Pad32,
    Pad33, Pad34, Pad35, Pad36, Pad37, Pad38, Pad39, Pad40,
    Pad41, Pad42, Pad43, Pad44, Pad45, Pad46, Pad47, Pad48,
    Pad49, Pad50, Pad51, Pad52, Pad53, Pad54, Pad55, Pad56,
    Pad57, Pad58, Pad59, Pad60, Pad61, Pad62, Pad63, Pad64,

    Arrange, Clear, Mst, Grp, Cue, Macro, Level, Aux, Auto, Perform, Variation, Lock, Tune,

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
  // clang-format on

  using tCbRender = std::function<void(void)>;
  using tCbDisconnect = std::function<void(void)>;

  using tCbButtonChanged = std::function<void(Button btn_, bool state_, bool shiftKey_)>;
  using tCbEncoderChanged = std::function<void(unsigned enc_, bool valIncreased_, bool shiftKey_)>;
  using tCbKeyChanged = std::function<void(unsigned index_, double, bool shiftKey_)>;
  using tCbControlChanged = std::function<void(unsigned pot_, double val_, bool shiftKey_)>;

  Device() = default;
  virtual ~Device() = default;

  void setDeviceHandle(tPtr<DeviceHandle> pDeviceHandle_);

  void resetDeviceHandle();

  virtual void init() = 0;

  virtual Canvas* graphicDisplay(size_t displayIndex_);

  virtual TextDisplay* textDisplay(size_t displayIndex_);

  virtual Canvas* ledMatrix(size_t ledMatrixIndex_);

  virtual LedArray* ledArray(size_t ledArrayIndex_);

  virtual size_t numOfGraphicDisplays() const = 0;

  virtual size_t numOfTextDisplays() const = 0;

  virtual size_t numOfLedMatrices() const = 0;

  virtual size_t numOfLedArrays() const = 0;

  virtual void setButtonLed(Button, const Color&);

  virtual void setKeyLed(unsigned, const Color&);

  virtual void sendMidiMsg(tRawData);

  void setCallbackDisconnect(tCbDisconnect cbDisconnect_);

  void setCallbackRender(tCbRender cbRender_);

  void setCallbackButtonChanged(tCbButtonChanged cbButtonChanged_);

  void setCallbackEncoderChanged(tCbEncoderChanged cbEncoderChanged_);

  void setCallbackKeyChanged(tCbKeyChanged cbKeyChanged_);

  void setCallbackControlChanged(tCbControlChanged cbControlChanged_);

  bool hasDeviceHandle();

protected:
  virtual bool tick() = 0;

  bool writeToDeviceHandle(const Transfer& transfer_, uint8_t endpoint_) const;

  bool readFromDeviceHandle(Transfer& transfer_, uint8_t endpoint_) const;

  void readFromDeviceHandleAsync(uint8_t endpoint_, DeviceHandle::tCbRead cbRead_) const;

  void buttonChanged(Button button_, bool buttonState_, bool shiftPressed_);

  void encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_);

  void keyChanged(unsigned index_, double value_, bool shiftPressed_);

  void controlChanged(unsigned potentiometer_, double value_, bool shiftPressed_);

private:
  bool onTick();

  void onConnect();

  void onDisconnect();

  void render();

  bool m_connected{false};
  tCbDisconnect m_cbDisconnect;
  tCbRender m_cbRender;

  tCbButtonChanged m_cbButtonChanged;
  tCbEncoderChanged m_cbEncoderChanged;
  tCbKeyChanged m_cbKeyChanged;
  tCbControlChanged m_cbControlChanged;

  mutable std::mutex m_mtxDeviceHandle;
  tPtr<DeviceHandle> m_pDeviceHandle;

  friend class Coordinator;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
