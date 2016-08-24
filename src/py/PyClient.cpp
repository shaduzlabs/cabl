/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "py/PyClient.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/*
useful links:
- https://misspent.wordpress.com/2009/10/11/boost-python-and-handling-python-exceptions/
- http://stackoverflow.com/questions/1418015/how-to-get-python-exception-text
*/

//--------------------------------------------------------------------------------------------------

std::string PyErrorString()
{
  try
  {
    PyObject *ptype, *value, *tb;
    boost::python::object formattedList;
    boost::python::object formatted;
    PyErr_Fetch(&ptype, &value, &tb);
    if (!ptype)
    {
      return "UNKNOWN error";
    }
    boost::python::handle<> hexc(ptype), hval(boost::python::allow_null(value)),
      htb(boost::python::allow_null(tb));
    boost::python::object traceback(boost::python::import("traceback"));
    boost::python::object format_exception_only(traceback.attr("format_exception_only"));
    formattedList = format_exception_only(hexc, hval);
    formatted = boost::python::str("\n").join(formattedList);
    return boost::python::extract<std::string>(formatted);
  }
  catch (...)
  {
    return "Error while trying to get python error";
  }
}

//--------------------------------------------------------------------------------------------------

using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

void PyClient::disconnected()
{
  GILLock gilLock;

  try
  {
    if (m_onDisconnected)
    {
      m_onDisconnected();
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::disconnected] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_)
{
  GILLock gilLock;

  try
  {
    if (m_onButtonChanged)
    {
      m_onButtonChanged(button_, buttonState_, shiftPressed_);
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::render] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
  GILLock gilLock;

  try
  {
    if (m_onEncoderChanged)
    {
      m_onEncoderChanged(encoder_, valueIncreased_, shiftPressed_);
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::render] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed_)
{
  GILLock gilLock;

  try
  {
    if (m_onPadChanged)
    {
      m_onPadChanged(pad_, value_, shiftPressed_);
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::render] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::keyChanged(Device::Key key_, uint16_t value_, bool shiftPressed_)
{
  GILLock gilLock;

  try
  {
    if (m_onKeyChanged)
    {
      m_onKeyChanged(key_, value_, shiftPressed_);
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::render] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::initDevice()
{
  GILLock gilLock;

  try
  {
    if (m_onInitDevice)
    {
      m_onInitDevice();
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::initDevice] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::render()
{
  GILLock gilLock;

  try
  {
    if (m_onRender)
    {
      m_onRender();
    }
  }
  catch (const error_already_set& e)
  {
    M_LOG("[PyClient::render] exception: " << PyErrorString());
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::setLed(Device::Button btn_, const util::LedColor& color_)
{
  if (device())
  {
    device()->setLed(btn_, color_);
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::setLed(Device::Pad pad_, const util::LedColor& color_)
{
  if (device())
  {
    device()->setLed(pad_, color_);
  }
}

//--------------------------------------------------------------------------------------------------

void PyClient::setLed(Device::Key key_, const util::LedColor& color_)
{
  if (device())
  {
    device()->setLed(key_, color_);
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
