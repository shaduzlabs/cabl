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

class Client
{
public:
  Client(DiscoveryPolicy = {});
  ~Client();
  
  void init();

  virtual void disconnected();
  virtual void buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_);
  virtual void encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_);
  virtual void keyChanged(unsigned index_, double value_, bool shiftPressed);
  virtual void controlChanged(unsigned pot_, double value_, bool shiftPressed);

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
    unsigned step_,
	unsigned currentValue_,
    unsigned minValue_,
	unsigned maxValue_);

  std::string m_clientId;
  Coordinator::tDevicePtr m_pDevice;
  DiscoveryPolicy m_discoveryPolicy;

  std::atomic<bool> m_update{true};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
