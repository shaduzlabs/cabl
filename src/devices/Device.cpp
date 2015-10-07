/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/Device.h"
#include "comm/DeviceHandle.h"

namespace sl
{
namespace kio
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

Device::Device(tPtr<DeviceHandle> pDeviceHandle_)
  : m_pDeviceHandle(std::move(pDeviceHandle_))
{

}
  
//--------------------------------------------------------------------------------------------------

Device::~Device()
{

}

//--------------------------------------------------------------------------------------------------

DeviceHandle* Device::getDeviceHandle()
{
  return m_pDeviceHandle.get();
}

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
