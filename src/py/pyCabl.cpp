/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/cabl.h"
#include "py/PyClient.h"

//--------------------------------------------------------------------------------------------------

// http://stackoverflow.com/questions/38261530/unresolved-external-symbols-since-visual-studio-2015-update-3-boost-python-link
namespace boost
{
template <>
sl::cabl::Canvas const volatile* get_pointer<class sl::cabl::Canvas const volatile>(
  class sl::cabl::Canvas const volatile* c)
{
  return c;
}

template <>
sl::cabl::TextDisplay const volatile* get_pointer<class sl::cabl::TextDisplay const volatile>(
  class sl::cabl::TextDisplay const volatile* c)
{
  return c;
}

template <>
sl::cabl::LedArray const volatile* get_pointer<class sl::cabl::LedArray const volatile>(
  class sl::cabl::LedArray const volatile* c)
{
  return c;
}
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace py
{

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

BOOST_PYTHON_MODULE(pycabl)
{
  Py_Initialize();
  PyEval_InitThreads();

  //------------------------------------------------------------------------------------------------

  class_<DiscoveryPolicy>("DiscoveryPolicy", init<std::string>())
    .def(init<std::string, DeviceDescriptor::tVendorId>())
    .def(init<std::string, DeviceDescriptor::tVendorId, DeviceDescriptor::tProductId>())
    .def(init<std::string,
      DeviceDescriptor::tVendorId,
      DeviceDescriptor::tProductId,
      DeviceDescriptor::Type>())
    .def("name", &DiscoveryPolicy::name)
    .def("type", &DiscoveryPolicy::type)
    .def("vendorId", &DiscoveryPolicy::vendorId)
    .def("productId", &DiscoveryPolicy::productId);

  //------------------------------------------------------------------------------------------------

  class_<PyClient, boost::noncopyable>("Client", init<object, object, object>())
    .def(init<object, object, object, DiscoveryPolicy>())
    .def("enumerateDevices", &enumerateDevices)
    .staticmethod("enumerateDevices")
    .def("onInit", &Client::initDevice)
    .def("setLedButton", &PyClient::setButtonLed)
    .def("setLedKey", &PyClient::setKeyLed)
    .def("onButtonChanged", &PyClient::onButtonChanged, args("onButtonChanged"))
    .def("onEncoderChanged", &PyClient::onEncoderChanged, args("onEncoderChanged"))
    .def("onKeyChanged", &PyClient::onKeyChanged, args("onKeyChanged"))
    .def("onControlChanged", &PyClient::onControlChanged, args("onControlChanged"))
    .def(
      "graphicDisplay", &PyClient::graphicDisplay, return_value_policy<reference_existing_object>())
    .def("textDisplay", &PyClient::textDisplay, return_value_policy<reference_existing_object>())
    .def("ledArray", &PyClient::ledArray, return_value_policy<reference_existing_object>())
    .def("ledMatrix", &PyClient::ledMatrix, return_value_policy<reference_existing_object>())
    .def("updateDevice", &PyClient::updateDevice);

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

  class_<Color>("Color")
    .def(init<uint8_t>())
    .def(init<uint8_t, uint8_t, uint8_t>())
    .def(init<uint8_t, uint8_t, uint8_t, uint8_t>())
    .def(self_ns::str(self_ns::self))
    .def("distance", &Color::distance)
    .def("getValue", &Color::getValue)
    .def("red", &Color::red)
    .def("green", &Color::green)
    .def("blue", &Color::blue)
    .def("mono", &Color::mono)
    .def("active", &Color::active)
    .def("blendMode", &Color::blendMode)
    .def("transparent", &Color::transparent)
    .def("setRed", &Color::setRed)
    .def("setGreen", &Color::setGreen)
    .def("setBlue", &Color::setBlue)
    .def("setMono", &Color::setMono)
    .def("setBlendMode", &Color::setBlendMode)
    .def("black", &Color::black)
    .def("white", &Color::white)
    .def("invert", &Color::invert);

  //------------------------------------------------------------------------------------------------

  class_<Canvas, boost::noncopyable>("Canvas", no_init)
    .def("width", &Canvas::width, "Returns the width of the display in pixels")
    .def("height", &Canvas::height, "Returns the height of the display in pixels")
    .def("pixel",
      &Canvas::pixel,
      args("pos"),
      "Returns the pixel value of the specified pixel as a Color object")
    .def("setPixel",
      &Canvas::setPixel,
      args("x", "y", "color"),
      "Sets the value of the pixel at x,y as a Color object")
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
      "Draws a triangle with the vertices in x0,y0 x1,y1 and x2,y2 using the specified colors "
      "for "
      "the border and the fill")
    .def("rectangle",
      &Canvas::rectangle,
      args("x", "y", "w", "h", "color"),
      "Draws a rectangle with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) using the "
      "specified "
      "color")
    .def("rectangleFilled",
      &Canvas::rectangleFilled,
      args("x", "y", "w", "h", "color", "fillColor"),
      "Draws a rectangle with the vertices in x,y (x+w),y (x+w),(y+h) x,(y+h) using the "
      "specified "
      "colors for the border and the fill")
    .def("rectangleRounded",
      &Canvas::rectangleRounded,
      args("x", "y", "w", "h", "r", "color"),
      "Draws a rectangle with rounded corners with the vertices in x,y (x+w),y (x+w),(y+h) "
      "x,(y+h) "
      "using the specified color")
    .def("rectangleRoundedFilled",
      &Canvas::rectangleRoundedFilled,
      args("x", "y", "w", "h", "r", "color"),
      "Draws a filled rectangle with rounded corners with the vertices in x,y (x+w),y "
      "(x+w),(y+h) "
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
      "Returns the value of the specified pixel as a Color object")
    .def("setPixel",
      &LedArray::setPixel,
      args("pos", "color"),
      "Sets the value of the specified pixel as a Color object")
    .def("setValue",
      &LedArray::setValue,
      args("val", "color", "alignment"),
      "Displays the value using the specified color and alignment");

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
