/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <boost/python.hpp>

#include "client/Client.h"
#include "devices/Coordinator.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

using namespace boost::python;
using namespace devices;

//--------------------------------------------------------------------------------------------------

class GILLock
{
public:
  GILLock()
  {
    m_state = PyGILState_Ensure();
  }

  ~GILLock()
  {
    PyGILState_Release(m_state);
  }

private:
  PyGILState_STATE m_state;
};

//--------------------------------------------------------------------------------------------------

class PyClient : public Client
{
public:
  PyClient(object fnInitDevice_, object fnRender_, object fnDisconnected_)
    : m_onInitDevice(fnInitDevice_), m_onRender(fnRender_), m_onDisconnected(fnDisconnected_)
  {
  }

  void disconnected() override;
  void buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_) override;
  void encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_) override;
  void padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed) override;
  void keyChanged(Device::Key key_, uint16_t value_, bool shiftPressed) override;
  void initDevice() override;
  void render() override;

  void onButtonChanged(object fn_)
  {
    m_onButtonChanged = fn_;
  }
  void onEncoderChanged(object fn_)
  {
    m_onEncoderChanged = fn_;
  }
  void onPadChanged(object fn_)
  {
    m_onPadChanged = fn_;
  }
  void onKeyChanged(object fn_)
  {
    m_onKeyChanged = fn_;
  }

  void setLed(Device::Button, const util::LedColor&);
  void setLed(Device::Pad, const util::LedColor&);
  void setLed(Device::Key, const util::LedColor&);

  GDisplay* displayGraphic(size_t displayIndex_)
  {
    return device()->displayGraphic(displayIndex_);
  }

private:
  object m_onInitDevice;
  object m_onRender;
  object m_onDisconnected;

  object m_onButtonChanged;
  object m_onEncoderChanged;
  object m_onPadChanged;
  object m_onKeyChanged;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
