/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "py/PyClient.h"
#include <cabl.h>

namespace sl
{
namespace cabl
{
namespace py
{

using namespace devices;

//--------------------------------------------------------------------------------------------------
/*
static object drawingContext_load(DrawingContext& self) {
  unsigned char* buffer;
  int size;
  self.load(buffer, size);

  //now you wrap that as buffer
  PyObject* py_buf = PyBuffer_FromReadWriteMemory(buffer, size);
  object retval = object(handle<>(py_buf));
  return retval;
}
*/

class CanvasHelper
{
public:
  static void write(Canvas* canvas_, uint8_t* buffer)
  {
    std::copy_n(buffer, canvas_->bufferSize(), canvas_->data());
  }
  static void setDirty(Canvas* display_)
  {
    display_->setDirty();
  }
  static unsigned dataSize(Canvas* canvas_)
  {
    return canvas_->bufferSize();
  }
  static unsigned canvasWidthInBytes(Canvas* canvas_)
  {
    return canvas_->canvasWidthInBytes();
  }
};

//--------------------------------------------------------------------------------------------------

static void writeToDisplay(Canvas& self_, object buffer)
{
  PyObject* pyBuffer = buffer.ptr();
  if (!PyBuffer_Check(pyBuffer))
  {
    // raise TypeError using standard boost::python mechanisms
  }
  PyObject* pobj = buffer.ptr();
  Py_buffer pybuf;
  PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE);

  CanvasHelper::write(&self_, static_cast<uint8_t*>(pybuf.buf));
  CanvasHelper::setDirty(&self_);
}

static void writeTextToDisplay(
  TextDisplay& self_, const std::string text_, unsigned row_, Alignment alignment_)
{
  self_.putText(text_, row_, alignment_);
}
//--------------------------------------------------------------------------------------------------

static unsigned displayDataSize(Canvas& self_)
{
  return CanvasHelper::dataSize(&self_);
}

//--------------------------------------------------------------------------------------------------

static unsigned canvasWidthInBytes(Canvas& self_)
{
  return CanvasHelper::canvasWidthInBytes(&self_);
}

//--------------------------------------------------------------------------------------------------

template <class T>
boost::python::list toPythonList(std::vector<T> vector)
{
  typename std::vector<T>::iterator iter;
  boost::python::list list;
  for (iter = vector.begin(); iter != vector.end(); ++iter)
  {
    list.append(*iter);
  }
  return list;
}

//--------------------------------------------------------------------------------------------------

struct NullDeleter
{
  void operator()(const void*)
  {
  }
};

//--------------------------------------------------------------------------------------------------

std::shared_ptr<DeviceFactory> deviceFactory()
{
  return std::shared_ptr<DeviceFactory>(&DeviceFactory::instance(), NullDeleter());
}

list enumerateDevices()
{
  return toPythonList(Coordinator::instance().enumerate());
}

//--------------------------------------------------------------------------------------------------

void onButtonChanged(PyObject* callable, Device::Button button_, bool val_, bool shiftKey_)
{
  boost::python::call<void>(callable, button_, val_, shiftKey_);
}

//--------------------------------------------------------------------------------------------------

void onEncoderChanged(PyObject* callable, Device::Encoder encoder_, uint16_t val_, bool shiftKey_)
{
  boost::python::call<void>(callable, encoder_, val_, shiftKey_);
}

//--------------------------------------------------------------------------------------------------

void onPadChanged(PyObject* callable, Device::Pad pad_, uint16_t val_, bool shiftKey_)
{
  boost::python::call<void>(callable, pad_, val_, shiftKey_);
}

//--------------------------------------------------------------------------------------------------

void onKeyChanged(PyObject* callable, Device::Key key_, uint16_t val_, bool shiftKey_)
{
  boost::python::call<void>(callable, key_, val_, shiftKey_);
}

//--------------------------------------------------------------------------------------------------

void onPotentiometerChanged(
  PyObject* callable, Device::Potentiometer pot_, uint16_t val_, bool shiftKey_)
{
  boost::python::call<void>(callable, pot_, val_, shiftKey_);
}

//--------------------------------------------------------------------------------------------------

BOOST_PYTHON_MODULE(pycabl)
{
  Py_Initialize();
  PyEval_InitThreads();

  //------------------------------------------------------------------------------------------------

  class_<DiscoveryPolicy>("DiscoveryPolicy",
    init<std::string,
                            DeviceDescriptor::tVendorId,
                            DeviceDescriptor::tProductId,
                            DeviceDescriptor::Type>())
    .def("name", &DiscoveryPolicy::name)
    .def("type", &DiscoveryPolicy::type)
    .def("vendorId", &DiscoveryPolicy::vendorId)
    .def("productId", &DiscoveryPolicy::productId);

  //------------------------------------------------------------------------------------------------

  void (PyClient::*setLed_btn)(Device::Button, const util::ColorRGB&) = &PyClient::setLed;
  void (PyClient::*setLed_pad)(Device::Pad, const util::ColorRGB&) = &PyClient::setLed;
  void (PyClient::*setLed_key)(Device::Key, const util::ColorRGB&) = &PyClient::setLed;

  class_<PyClient, boost::noncopyable>("Client", init<object, object, object>())
    .def("enumerateDevices", &enumerateDevices)
    .staticmethod("enumerateDevices")
    .def("onInit", &Client::initDevice)
    .def("setLedButton", setLed_btn)
    .def("setLedPad", setLed_pad)
    .def("setLedKey", setLed_key)
    .def("onButtonChanged", &PyClient::onButtonChanged, args("onButtonChanged"))
    .def("onPadChanged", &PyClient::onPadChanged, args("onPadChanged"))
    .def("onEncoderChanged", &PyClient::onEncoderChanged, args("onEncoderChanged"))
    .def("onKeyChanged", &PyClient::onKeyChanged, args("onKeyChanged"))
    .def(
      "graphicDisplay", &PyClient::graphicDisplay, return_value_policy<reference_existing_object>())
    .def("textDisplay", &PyClient::textDisplay, return_value_policy<reference_existing_object>())
    .def("ledArray", &PyClient::ledArray, return_value_policy<reference_existing_object>())
    .def("ledMatrix", &PyClient::ledMatrix, return_value_policy<reference_existing_object>());

  //------------------------------------------------------------------------------------------------

  class_<DeviceDescriptor>("DeviceDescriptor",
    init<std::string,
                             DeviceDescriptor::Type,
                             DeviceDescriptor::tVendorId,
                             DeviceDescriptor::tProductId>())
    .def(self_ns::str(self_ns::self))
    .def("name", &DeviceDescriptor::name, return_value_policy<copy_const_reference>())
    .def("type", &DeviceDescriptor::type)
    .def("vendorId", &DeviceDescriptor::vendorId)
    .def("productId", &DeviceDescriptor::productId)
    .def(
      "serialNumber", &DeviceDescriptor::serialNumber, return_value_policy<copy_const_reference>());

  //------------------------------------------------------------------------------------------------

  class_<util::ColorRGB>("ColorRGB", init<uint8_t, uint8_t, uint8_t>())
    .def(init<uint8_t, uint8_t, uint8_t>())
    .def(init<uint8_t, uint8_t, uint8_t, uint8_t>())
    .def(self_ns::str(self_ns::self))
    .def("distance", &util::ColorRGB::distance)
    .def("getValue", &util::ColorRGB::getValue)
    .def("red", &util::ColorRGB::red)
    .def("green", &util::ColorRGB::green)
    .def("blue", &util::ColorRGB::blue)
    .def("mono", &util::ColorRGB::mono)
    .def("active", &util::ColorRGB::active)
    .def("blendMode", &util::ColorRGB::blendMode)
    .def("transparent", &util::ColorRGB::transparent)
    .def("setRed", &util::ColorRGB::setRed)
    .def("setGreen", &util::ColorRGB::setGreen)
    .def("setBlue", &util::ColorRGB::setBlue)
    .def("setMono", &util::ColorRGB::setMono)
    .def("setBlendMode", &util::ColorRGB::setBlendMode)
    .def("black", &util::ColorRGB::black)
    .def("white", &util::ColorRGB::white)
    .def("invert", &util::ColorRGB::invert);

  //------------------------------------------------------------------------------------------------

  class_<Canvas, boost::noncopyable>("Canvas", no_init)
    .def("width", &Canvas::width, "Returns the width of the display in pixels")
    .def("height", &Canvas::height, "Returns the height of the display in pixels")
    .def("pixel",
      &Canvas::pixel,
      args("pos"),
      "Returns the pixel value of the specified pixel as a ColorRGB object")
    .def("setPixel",
      &Canvas::setPixel,
      args("x", "y", "color"),
      "Sets the value of the pixel at x,y as a ColorRGB object")
    .def("black", &Canvas::black, "Fills the display with black")
    .def("white", &Canvas::white, "Fills the display with white")
    .def("invert", &Canvas::invert, "Inverts the content of the display")
    .def("fill", &Canvas::fill, "Fills the display with the specified value")
    .def("line",
      &Canvas::line,
      args("x0", "y0", "x1", "y1", "color"),
      "Draws a line between x0,y0 and x1,y1 using the specified color")
    .def("lineVertical",
      &Canvas::lineVertical,
      args("x", "y", "length", "color"),
      "Draws a vertical line between x,y and x,(y+l) using the specified color")
    .def("lineHorizontal",
      &Canvas::lineHorizontal,
      args("x", "y", "length", "color"),
      "Draws an horizontal line between x,y and (x+l),y using the specified color")
    .def("triangle",
      &Canvas::triangle,
      args("x0", "y0", "x1", "y1", "x2", "y2", "color"),
      "Draws a triangle with the vertices in x0,y0 x1,y1 and x2,y2 using the specified color")
    .def("triangleFilled",
      &Canvas::triangleFilled,
      args("x0", "y0", "x1", "y1", "x2", "y2", "color", "fillColor"),
      "Draws a triangle with the vertices in x0,y0 x1,y1 and x2,y2 using the specified colors for "
      "the border and the fill")
    .def("rectangle",
      &Canvas::rectangle,
      args("x", "y", "w", "h", "color"),
      "Draws a rectangle with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) using the specified "
      "color")
    .def("rectangleFilled",
      &Canvas::rectangleFilled,
      args("x", "y", "w", "h", "color", "fillColor"),
      "Draws a rectangle with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) using the specified "
      "colors for the border and the fill")
    .def("rectangleRounded",
      &Canvas::rectangleRounded,
      args("x", "y", "w", "h", "r", "color"),
      "Draws a rectangle with rounded corners with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) "
      "using the specified color")
    .def("rectangleRoundedFilled",
      &Canvas::rectangleRoundedFilled,
      args("x", "y", "w", "h", "r", "color"),
      "Draws a filled rectangle with rounded corners with the vertices in x,y (x+w),y (x+w),(y+h) "
      "x,(y+h) using the specified colors for the border and the fill")
    .def("circle",
      &Canvas::circle,
      args("x", "y", "r", "color", "type"),
      "Draws a circle with center in x,y and radius r using the specified color. The circle type "
      "can also be specified and is defaulted to Full")
    .def("circleFilled",
      &Canvas::circleFilled,
      args("x", "y", "r", "color", "fillColor", "type"),
      "Draws a circle with center in x,y and radius r using the specified colors for the border "
      "and the fill. The circle type can also be specified and is defaulted to Full.")
    .def("putBitmap",
      &Canvas::putBitmap,
      args("x", "y", "w", "h", "bitmap", "color"),
      "Draws a 1-bit bitmap with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) using the "
      "specified color")
    .def("putCanvas",
      &Canvas::putCanvas,
      args("canvas", "xdest", "ydest", "xsource", "ysource", "w", "h"),
      "Draws a part of the canvas c identified by xsource,ysource (xsource+w),ysource "
      "(xsource+w),(ysource+h) xsource,(ysource+h) starting at xdest,ydest")
    .def("putCharacter",
      &Canvas::putCharacter,
      args("x", "y", "c", "color", "font"),
      "Draws a character c at x,y using the specified color and font")
    .def("putText",
      &Canvas::putText,
      args("x", "y", "text", "color", "font", "spacing"),
      "Draws a string at x,y using the specified color and font. The spacing between characters "
      "can also be specified and defaults to 0")
    .def("dataSize", &displayDataSize, "Returns the display buffer size in bytes")
    .def("canvasWidthInBytes", &canvasWidthInBytes, "Returns the display width in bytes")
    .def("write", &writeToDisplay, args("buffer"), "Write a raw buffer to the display");

  //------------------------------------------------------------------------------------------------

  class_<TextDisplay, boost::noncopyable>("TextDisplay", no_init)
    .def("width", &Canvas::width, "Returns the width of the display in number of characters")
    .def("height", &Canvas::height, "Returns the height of the display in rows")
    .def("clear", &TextDisplay::clear, "Clears the display")
    .def("putCharacter",
      &TextDisplay::putCharacter,
      args("col", "row", "c"),
      "Displays a character c at col,row")
    .def("putText",
      &writeTextToDisplay,
      args("string", "row", "alignment"),
      "Displays a string at the specified row using the specified alignment");

  //------------------------------------------------------------------------------------------------

  class_<LedArray, boost::noncopyable>("LedArray", no_init)
    .def("length", &LedArray::length, "Returns the number of leds in the array")
    .def("pixel",
      &LedArray::pixel,
      args("pos"),
      "Returns the value of the specified pixel as a ColorRGB object")
    .def("setPixel",
      &LedArray::setPixel,
      args("pos", "color"),
      "Sets the value of the specified pixel as a ColorRGB object");

  //------------------------------------------------------------------------------------------------

#define M_DESCRIPTOR_TYPE_DEF(item) value(#item, DeviceDescriptor::Type::item)
  enum_<DeviceDescriptor::Type>("DeviceDescriptorType")
    .M_DESCRIPTOR_TYPE_DEF(USB)
    .M_DESCRIPTOR_TYPE_DEF(MIDI)
    .M_DESCRIPTOR_TYPE_DEF(HID)
    .M_DESCRIPTOR_TYPE_DEF(Unknown);
#undef M_DESCRIPTOR_TYPE_DEF

  //------------------------------------------------------------------------------------------------

#define M_BTN_DEF(item) value(#item, Device::Button::item)
  enum_<Device::Button>("Button")
    .M_BTN_DEF(Control)
    .M_BTN_DEF(Step)
    .M_BTN_DEF(Browse)
    .M_BTN_DEF(Sampling)
    .M_BTN_DEF(BrowseLeft)
    .M_BTN_DEF(BrowseRight)
    .M_BTN_DEF(AutoWrite)
    .M_BTN_DEF(F2)
    .M_BTN_DEF(All)
    .M_BTN_DEF(F1)
    .M_BTN_DEF(Snap)
    .M_BTN_DEF(DisplayButton1)
    .M_BTN_DEF(DisplayButton2)
    .M_BTN_DEF(DisplayButton3)
    .M_BTN_DEF(DisplayButton4)
    .M_BTN_DEF(DisplayButton5)
    .M_BTN_DEF(DisplayButton6)
    .M_BTN_DEF(DisplayButton7)
    .M_BTN_DEF(DisplayButton8)
    .M_BTN_DEF(Scene)
    .M_BTN_DEF(Pattern)
    .M_BTN_DEF(PadMode)
    .M_BTN_DEF(Keyboard)
    .M_BTN_DEF(View)
    .M_BTN_DEF(Navigate)
    .M_BTN_DEF(Duplicate)
    .M_BTN_DEF(Select)
    .M_BTN_DEF(Solo)
    .M_BTN_DEF(Mute)
    .M_BTN_DEF(Volume)
    .M_BTN_DEF(Swing)
    .M_BTN_DEF(Tempo)
    .M_BTN_DEF(MasterLeft)
    .M_BTN_DEF(MasterRight)
    .M_BTN_DEF(Enter)
    .M_BTN_DEF(Tap)
    .M_BTN_DEF(NoteRepeat)
    .M_BTN_DEF(Group)
    .M_BTN_DEF(GroupA)
    .M_BTN_DEF(GroupB)
    .M_BTN_DEF(GroupC)
    .M_BTN_DEF(GroupD)
    .M_BTN_DEF(GroupE)
    .M_BTN_DEF(GroupF)
    .M_BTN_DEF(GroupG)
    .M_BTN_DEF(GroupH)
    .M_BTN_DEF(Restart)
    .M_BTN_DEF(Loop)
    .M_BTN_DEF(TransportLeft)
    .M_BTN_DEF(TransportRight)
    .M_BTN_DEF(Grid)
    .M_BTN_DEF(Play)
    .M_BTN_DEF(Rec)
    .M_BTN_DEF(Erase)
    .M_BTN_DEF(Shift)
    .M_BTN_DEF(F3)
    .M_BTN_DEF(Nav)
    .M_BTN_DEF(Main)
    .M_BTN_DEF(MainEncoder)
    .M_BTN_DEF(Scale)
    .M_BTN_DEF(Scales)
    .M_BTN_DEF(Arp)
    .M_BTN_DEF(Rwd)
    .M_BTN_DEF(Ffw)
    .M_BTN_DEF(Stop)
    .M_BTN_DEF(PageLeft)
    .M_BTN_DEF(PageRight)
    .M_BTN_DEF(PresetUp)
    .M_BTN_DEF(Instance)
    .M_BTN_DEF(PresetDown)
    .M_BTN_DEF(Back)
    .M_BTN_DEF(NavigateUp)
    .M_BTN_DEF(NavigateLeft)
    .M_BTN_DEF(NavigateDown)
    .M_BTN_DEF(NavigateRight)
    .M_BTN_DEF(OctaveUp)
    .M_BTN_DEF(OctaveDown)
    .M_BTN_DEF(TouchEncoder1)
    .M_BTN_DEF(TouchEncoder2)
    .M_BTN_DEF(TouchEncoder3)
    .M_BTN_DEF(TouchEncoder4)
    .M_BTN_DEF(TouchEncoder5)
    .M_BTN_DEF(TouchEncoder6)
    .M_BTN_DEF(TouchEncoder7)
    .M_BTN_DEF(TouchEncoder8)
    .M_BTN_DEF(TouchEncoder9)
    .M_BTN_DEF(TouchEncoderMain)
    .M_BTN_DEF(TouchEncoderMain2)
    .M_BTN_DEF(Size)
    .M_BTN_DEF(Type)
    .M_BTN_DEF(Reverse)
    .M_BTN_DEF(Capture)
    .M_BTN_DEF(Quant)
    .M_BTN_DEF(Quantize)
    .M_BTN_DEF(Sync)
    .M_BTN_DEF(Pad1)
    .M_BTN_DEF(Pad2)
    .M_BTN_DEF(Pad3)
    .M_BTN_DEF(Pad4)
    .M_BTN_DEF(Pad5)
    .M_BTN_DEF(Pad6)
    .M_BTN_DEF(Pad7)
    .M_BTN_DEF(Pad8)
    .M_BTN_DEF(Pad9)
    .M_BTN_DEF(Pad10)
    .M_BTN_DEF(Pad11)
    .M_BTN_DEF(Pad12)
    .M_BTN_DEF(Pad13)
    .M_BTN_DEF(Pad14)
    .M_BTN_DEF(Pad15)
    .M_BTN_DEF(Pad16)
    .M_BTN_DEF(Stop1)
    .M_BTN_DEF(Stop2)
    .M_BTN_DEF(Stop3)
    .M_BTN_DEF(Stop4)
    .M_BTN_DEF(Btn1Row1)
    .M_BTN_DEF(Btn2Row1)
    .M_BTN_DEF(Btn3Row1)
    .M_BTN_DEF(Btn4Row1)
    .M_BTN_DEF(Btn5Row1)
    .M_BTN_DEF(Btn6Row1)
    .M_BTN_DEF(Btn7Row1)
    .M_BTN_DEF(Btn8Row1)
    .M_BTN_DEF(Btn1Row2)
    .M_BTN_DEF(Btn2Row2)
    .M_BTN_DEF(Btn3Row2)
    .M_BTN_DEF(Btn4Row2)
    .M_BTN_DEF(Btn5Row2)
    .M_BTN_DEF(Btn6Row2)
    .M_BTN_DEF(Btn7Row2)
    .M_BTN_DEF(Btn8Row2)
    .M_BTN_DEF(Grid1_4)
    .M_BTN_DEF(Grid1_4T)
    .M_BTN_DEF(Grid1_8)
    .M_BTN_DEF(Grid1_8T)
    .M_BTN_DEF(Grid1_16)
    .M_BTN_DEF(Grid1_16T)
    .M_BTN_DEF(Grid1_32)
    .M_BTN_DEF(Grid1_32T)
    .M_BTN_DEF(TapTempo)
    .M_BTN_DEF(Metronome)
    .M_BTN_DEF(TouchStripTap)
    .M_BTN_DEF(Master)
    .M_BTN_DEF(Setup)
    .M_BTN_DEF(Layout)
    .M_BTN_DEF(Convert)
    .M_BTN_DEF(Note)
    .M_BTN_DEF(Session)
    .M_BTN_DEF(AddEffect)
    .M_BTN_DEF(AddTrack)
    .M_BTN_DEF(Repeat)
    .M_BTN_DEF(Accent)
    .M_BTN_DEF(User)
    .M_BTN_DEF(In)
    .M_BTN_DEF(Out)
    .M_BTN_DEF(New)
    .M_BTN_DEF(Automation)
    .M_BTN_DEF(FixedLength)
    .M_BTN_DEF(Device)
    .M_BTN_DEF(Track)
    .M_BTN_DEF(Clip)
    .M_BTN_DEF(PanSend)
    .M_BTN_DEF(Double)
    .M_BTN_DEF(Delete)
    .M_BTN_DEF(Undo)
    .M_BTN_DEF(Unknown);
#undef M_BTN_DEF

  //------------------------------------------------------------------------------------------------

#define M_ENCODER_DEF(item) value(#item, Device::Encoder::item)
  enum_<Device::Encoder>("Encoder")
    .M_ENCODER_DEF(Main)
    .M_ENCODER_DEF(Volume)
    .M_ENCODER_DEF(Tempo)
    .M_ENCODER_DEF(Main2)
    .M_ENCODER_DEF(Swing)
    .M_ENCODER_DEF(Encoder1)
    .M_ENCODER_DEF(Encoder2)
    .M_ENCODER_DEF(Encoder3)
    .M_ENCODER_DEF(Encoder4)
    .M_ENCODER_DEF(Encoder5)
    .M_ENCODER_DEF(Encoder6)
    .M_ENCODER_DEF(Encoder7)
    .M_ENCODER_DEF(Encoder8)
    .M_ENCODER_DEF(Encoder9)
    .M_ENCODER_DEF(Unknown);
#undef M_ENCODER_DEF

  //------------------------------------------------------------------------------------------------

#define M_PAD_DEF(item) value(#item, Device::Pad::item)
  enum_<Device::Pad>("Pad")
    .M_PAD_DEF(Pad1)
    .M_PAD_DEF(Pad2)
    .M_PAD_DEF(Pad3)
    .M_PAD_DEF(Pad4)
    .M_PAD_DEF(Pad5)
    .M_PAD_DEF(Pad6)
    .M_PAD_DEF(Pad7)
    .M_PAD_DEF(Pad8)
    .M_PAD_DEF(Pad9)
    .M_PAD_DEF(Pad10)
    .M_PAD_DEF(Pad11)
    .M_PAD_DEF(Pad12)
    .M_PAD_DEF(Pad13)
    .M_PAD_DEF(Pad14)
    .M_PAD_DEF(Pad15)
    .M_PAD_DEF(Pad16)
    .M_PAD_DEF(Pad17)
    .M_PAD_DEF(Pad18)
    .M_PAD_DEF(Pad19)
    .M_PAD_DEF(Pad20)
    .M_PAD_DEF(Pad21)
    .M_PAD_DEF(Pad22)
    .M_PAD_DEF(Pad23)
    .M_PAD_DEF(Pad24)
    .M_PAD_DEF(Pad25)
    .M_PAD_DEF(Pad26)
    .M_PAD_DEF(Pad27)
    .M_PAD_DEF(Pad28)
    .M_PAD_DEF(Pad29)
    .M_PAD_DEF(Pad30)
    .M_PAD_DEF(Pad31)
    .M_PAD_DEF(Pad32)
    .M_PAD_DEF(Pad33)
    .M_PAD_DEF(Pad34)
    .M_PAD_DEF(Pad35)
    .M_PAD_DEF(Pad36)
    .M_PAD_DEF(Pad37)
    .M_PAD_DEF(Pad38)
    .M_PAD_DEF(Pad39)
    .M_PAD_DEF(Pad40)
    .M_PAD_DEF(Pad41)
    .M_PAD_DEF(Pad42)
    .M_PAD_DEF(Pad43)
    .M_PAD_DEF(Pad44)
    .M_PAD_DEF(Pad45)
    .M_PAD_DEF(Pad46)
    .M_PAD_DEF(Pad47)
    .M_PAD_DEF(Pad48)
    .M_PAD_DEF(Pad49)
    .M_PAD_DEF(Pad50)
    .M_PAD_DEF(Pad51)
    .M_PAD_DEF(Pad52)
    .M_PAD_DEF(Pad53)
    .M_PAD_DEF(Pad54)
    .M_PAD_DEF(Pad55)
    .M_PAD_DEF(Pad56)
    .M_PAD_DEF(Pad57)
    .M_PAD_DEF(Pad58)
    .M_PAD_DEF(Pad59)
    .M_PAD_DEF(Pad60)
    .M_PAD_DEF(Pad61)
    .M_PAD_DEF(Pad62)
    .M_PAD_DEF(Pad63)
    .M_PAD_DEF(Pad64)
    .M_PAD_DEF(Unknown);
#undef M_PAD_DEF

  //------------------------------------------------------------------------------------------------

#define M_KEY_DEF(item) value(#item, Device::Key::item)
  enum_<Device::Key>("Key")
    .M_KEY_DEF(Key1)
    .M_KEY_DEF(Key2)
    .M_KEY_DEF(Key3)
    .M_KEY_DEF(Key4)
    .M_KEY_DEF(Key5)
    .M_KEY_DEF(Key6)
    .M_KEY_DEF(Key7)
    .M_KEY_DEF(Key8)
    .M_KEY_DEF(Key9)
    .M_KEY_DEF(Key10)
    .M_KEY_DEF(Key11)
    .M_KEY_DEF(Key12)
    .M_KEY_DEF(Key13)
    .M_KEY_DEF(Key14)
    .M_KEY_DEF(Key15)
    .M_KEY_DEF(Key16)
    .M_KEY_DEF(Key17)
    .M_KEY_DEF(Key18)
    .M_KEY_DEF(Key19)
    .M_KEY_DEF(Key20)
    .M_KEY_DEF(Key21)
    .M_KEY_DEF(Key22)
    .M_KEY_DEF(Key23)
    .M_KEY_DEF(Key24)
    .M_KEY_DEF(Key25)
    .M_KEY_DEF(Key26)
    .M_KEY_DEF(Key27)
    .M_KEY_DEF(Key28)
    .M_KEY_DEF(Key29)
    .M_KEY_DEF(Key30)
    .M_KEY_DEF(Key31)
    .M_KEY_DEF(Key32)
    .M_KEY_DEF(Key33)
    .M_KEY_DEF(Key34)
    .M_KEY_DEF(Key35)
    .M_KEY_DEF(Key36)
    .M_KEY_DEF(Key37)
    .M_KEY_DEF(Key38)
    .M_KEY_DEF(Key39)
    .M_KEY_DEF(Key40)
    .M_KEY_DEF(Key41)
    .M_KEY_DEF(Key42)
    .M_KEY_DEF(Key43)
    .M_KEY_DEF(Key44)
    .M_KEY_DEF(Key45)
    .M_KEY_DEF(Key46)
    .M_KEY_DEF(Key47)
    .M_KEY_DEF(Key48)
    .M_KEY_DEF(Key49)
    .M_KEY_DEF(Key50)
    .M_KEY_DEF(Key51)
    .M_KEY_DEF(Key52)
    .M_KEY_DEF(Key53)
    .M_KEY_DEF(Key54)
    .M_KEY_DEF(Key55)
    .M_KEY_DEF(Key56)
    .M_KEY_DEF(Key57)
    .M_KEY_DEF(Key58)
    .M_KEY_DEF(Key59)
    .M_KEY_DEF(Key60)
    .M_KEY_DEF(Key61)
    .M_KEY_DEF(Key62)
    .M_KEY_DEF(Key63)
    .M_KEY_DEF(Key64)
    .M_KEY_DEF(Key65)
    .M_KEY_DEF(Key66)
    .M_KEY_DEF(Key67)
    .M_KEY_DEF(Key68)
    .M_KEY_DEF(Key69)
    .M_KEY_DEF(Key70)
    .M_KEY_DEF(Key71)
    .M_KEY_DEF(Key72)
    .M_KEY_DEF(Key73)
    .M_KEY_DEF(Key74)
    .M_KEY_DEF(Key75)
    .M_KEY_DEF(Key76)
    .M_KEY_DEF(Key77)
    .M_KEY_DEF(Key78)
    .M_KEY_DEF(Key79)
    .M_KEY_DEF(Key80)
    .M_KEY_DEF(Key81)
    .M_KEY_DEF(Key82)
    .M_KEY_DEF(Key83)
    .M_KEY_DEF(Key84)
    .M_KEY_DEF(Key85)
    .M_KEY_DEF(Key86)
    .M_KEY_DEF(Key87)
    .M_KEY_DEF(Key88)
    .M_KEY_DEF(Key89)
    .M_KEY_DEF(Key90)
    .M_KEY_DEF(Key91)
    .M_KEY_DEF(Key92)
    .M_KEY_DEF(Key93)
    .M_KEY_DEF(Key94)
    .M_KEY_DEF(Key95)
    .M_KEY_DEF(Key96)
    .M_KEY_DEF(Key97)
    .M_KEY_DEF(Key98)
    .M_KEY_DEF(Key99)
    .M_KEY_DEF(Key100)
    .M_KEY_DEF(Key101)
    .M_KEY_DEF(Key102)
    .M_KEY_DEF(Key103)
    .M_KEY_DEF(Key104)
    .M_KEY_DEF(Key105)
    .M_KEY_DEF(Key106)
    .M_KEY_DEF(Key107)
    .M_KEY_DEF(Key108)
    .M_KEY_DEF(Key109)
    .M_KEY_DEF(Key110)
    .M_KEY_DEF(Key111)
    .M_KEY_DEF(Key112)
    .M_KEY_DEF(Key113)
    .M_KEY_DEF(Key114)
    .M_KEY_DEF(Key115)
    .M_KEY_DEF(Key116)
    .M_KEY_DEF(Key117)
    .M_KEY_DEF(Key118)
    .M_KEY_DEF(Key119)
    .M_KEY_DEF(Key120)
    .M_KEY_DEF(Key121)
    .M_KEY_DEF(Key122)
    .M_KEY_DEF(Key123)
    .M_KEY_DEF(Key124)
    .M_KEY_DEF(Key125)
    .M_KEY_DEF(Key126)
    .M_KEY_DEF(Key127)
    .M_KEY_DEF(Key128)
    .M_KEY_DEF(Unknown);
#undef M_KEY_DEF

  //------------------------------------------------------------------------------------------------

#define M_POT_DEF(item) value(#item, Device::Potentiometer::item)
  enum_<Device::Potentiometer>("Potentiometer")
    .M_POT_DEF(CenterDetented1)
    .M_POT_DEF(CenterDetented2)
    .M_POT_DEF(CenterDetented3)
    .M_POT_DEF(CenterDetented4)
    .M_POT_DEF(CenterDetented5)
    .M_POT_DEF(CenterDetented6)
    .M_POT_DEF(CenterDetented7)
    .M_POT_DEF(CenterDetented8)
    .M_POT_DEF(Fader1)
    .M_POT_DEF(Fader2)
    .M_POT_DEF(Fader3)
    .M_POT_DEF(Fader4)
    .M_POT_DEF(Fader5)
    .M_POT_DEF(Fader6)
    .M_POT_DEF(Fader7)
    .M_POT_DEF(Fader8)
    .M_POT_DEF(Fader9)
    .M_POT_DEF(Fader10)
    .M_POT_DEF(Fader11)
    .M_POT_DEF(Fader12)
    .M_POT_DEF(Fader13)
    .M_POT_DEF(Fader14)
    .M_POT_DEF(Fader15)
    .M_POT_DEF(Fader16)
    .M_POT_DEF(Unknown);
#undef M_POT_DEF

  //------------------------------------------------------------------------------------------------

#define M_CIRCLE_DEF(item) value(#item, Canvas::CircleType::item)
  enum_<Canvas::CircleType>("CircleType")
    .M_CIRCLE_DEF(Full)
    .M_CIRCLE_DEF(SemiLeft)
    .M_CIRCLE_DEF(SemiTop)
    .M_CIRCLE_DEF(SemiRight)
    .M_CIRCLE_DEF(SemiBottom)
    .M_CIRCLE_DEF(QuarterTopLeft)
    .M_CIRCLE_DEF(QuarterTopRight)
    .M_CIRCLE_DEF(QuarterBottomRight)
    .M_CIRCLE_DEF(QuarterBottomLeft);
#undef M_CIRCLE_DEF

  //------------------------------------------------------------------------------------------------

#define M_BLENDMODE_DEF(item) value(#item, BlendMode::item)
  enum_<BlendMode>("BlendMode")
    .M_BLENDMODE_DEF(Normal)
    .M_BLENDMODE_DEF(Invert)
    .M_BLENDMODE_DEF(Transparent);
#undef M_BLENDMODE_DEF

  //------------------------------------------------------------------------------------------------

#define M_ALIGNMENT_DEF(item) value(#item, Alignment::item)
  enum_<Alignment>("Alignment").M_ALIGNMENT_DEF(Left).M_ALIGNMENT_DEF(Center).M_ALIGNMENT_DEF(Right);
#undef M_ALIGNMENT_DEF

  //------------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------

} // namespace py
} // namespace cabl
} // namespace sl
