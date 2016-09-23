/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/devices/DeviceFactory.h"
#include "cabl/devices/Device.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

std::shared_ptr<Device> DeviceFactory::device(
  const DeviceDescriptor& deviceDescriptor_, tPtr<DeviceHandle> pDeviceHandle_)
{
  for (const auto& dd : m_registry)
  {
    if (dd.first.isSameProduct(deviceDescriptor_))
    {
      auto device = dd.second();
      device->setDeviceHandle(std::move(pDeviceHandle_));
      return device;
    }
  }

  return nullptr;
}

//--------------------------------------------------------------------------------------------------

bool DeviceFactory::isKnownDevice(const DeviceDescriptor& deviceDescriptor_) const
{
  for (const auto& dd : m_registry)
  {
    if (dd.first.isSameProduct(deviceDescriptor_))
    {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

void DeviceFactory::registerClass(const DeviceDescriptor& deviceDescriptor_, tFnCreate fnCreate_)
{
  m_registry.insert(std::pair<DeviceDescriptor, tFnCreate>(deviceDescriptor_, fnCreate_));
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
