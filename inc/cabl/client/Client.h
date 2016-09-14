/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/comm/DiscoveryPolicy.h"
#include "cabl/devices/Coordinator.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

using namespace devices;

class Client
{
public:
  Client(DiscoveryPolicy = {});
  ~Client();

  virtual void disconnected();
  virtual void buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_);
  virtual void encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_);
  virtual void padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed);
  virtual void keyChanged(Device::Key key_, uint16_t value_, bool shiftPressed);
  virtual void potentiometerChanged(Device::Potentiometer pot_, uint16_t value_, bool shiftPressed);

  virtual void initDevice()
  {
  }
  virtual void render() = 0;

protected:
  Coordinator::tDevicePtr device()
  {
    return m_pDevice;
  }
  void requestDeviceUpdate()
  {
    m_update = true;
  }

private:
  void onInitDevice();
  void onRender();

  void devicesListChanged(Coordinator::tCollDeviceDescriptor devices_);

  uint8_t encoderValue(bool valueIncreased_,
    uint8_t step_,
    uint8_t currentValue_,
    uint8_t minValue_,
    uint8_t maxValue_);

  std::string m_clientId;
  Coordinator::tDevicePtr m_pDevice;
  DiscoveryPolicy m_discoveryPolicy;

  std::atomic<bool> m_update{true};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
