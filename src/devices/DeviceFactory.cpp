/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/DeviceFactory.h"
#include "devices/Device.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

std::shared_ptr<Device> DeviceFactory::getDevice(
  const DeviceDescriptor& deviceDescriptor_, 
  tPtr<DeviceHandle> pDeviceHandle_
)
{
  auto deviceClass = m_registry.find(deviceDescriptor_);
  if( deviceClass == m_registry.end() )
  {
    return nullptr;
  }
  auto device = deviceClass->second();
  device->setDeviceHandle(std::move(pDeviceHandle_));
  return device;
}

//--------------------------------------------------------------------------------------------------

bool DeviceFactory::isKnownDevice(const DeviceDescriptor& deviceDescriptor_) const
{
  return m_registry.find(deviceDescriptor_) != m_registry.end();
}

//--------------------------------------------------------------------------------------------------

void DeviceFactory::registerClass(
  const DeviceDescriptor& deviceDescriptor_,
  tFnCreate fnCreate_
)
{
  m_registry.insert(std::pair<DeviceDescriptor, tFnCreate>(deviceDescriptor_, fnCreate_)); 
}

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
