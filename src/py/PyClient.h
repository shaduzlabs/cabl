/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <boost/python.hpp>

#include "cabl/client/Client.h"
#include "cabl/devices/Coordinator.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace py
{

//--------------------------------------------------------------------------------------------------

using namespace boost::python;

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
  PyClient(object fnInitDevice_,
    object fnRender_,
    object fnDisconnected_,
    DiscoveryPolicy discoveryPolicy_ = {});

  void disconnected() override;
  void buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_) override;
  void encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_) override;
  void keyChanged(unsigned index_, double value_, bool shiftPressed) override;
  void controlChanged(unsigned pot_, double value_, bool shiftPressed) override;

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
  void onKeyChanged(object fn_)
  {
    m_onKeyChanged = fn_;
  }
  void onControlChanged(object fn_)
  {
    m_onControlChanged = fn_;
  }
  void setButtonLed(Device::Button, const Color&);
  void setKeyLed(unsigned, const Color&);

  Canvas* graphicDisplay(size_t displayIndex_)
  {
    return device()->graphicDisplay(displayIndex_);
  }
  TextDisplay* textDisplay(size_t displayIndex_)
  {
    return device()->textDisplay(displayIndex_);
  }
  Canvas* ledMatrix(size_t ledMatrixIndex_)
  {
    return device()->ledMatrix(ledMatrixIndex_);
  }
  LedArray* ledArray(size_t ledArrayIndex_)
  {
    return device()->ledArray(ledArrayIndex_);
  }

  void updateDevice()
  {
    requestDeviceUpdate();
  }

  void writeToDisplay()
  {
  }

private:
  object m_onInitDevice;
  object m_onRender;
  object m_onDisconnected;

  object m_onButtonChanged;
  object m_onEncoderChanged;
  object m_onKeyChanged;
  object m_onControlChanged;
};

//--------------------------------------------------------------------------------------------------

} // namespace py
} // namespace cabl
} // namespace sl
