/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/SAM3X8E/DriverSAM3X8E.h"
#include "comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DriverSAM3X8E::DriverSAM3X8E()
{

}

//--------------------------------------------------------------------------------------------------

DriverSAM3X8E::~DriverSAM3X8E()
{

}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverSAM3X8E::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverSAM3X8E::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

uint32_t DriverSAM3X8E::Init(uint32_t parent, uint32_t port, uint32_t lowspeed)
{
  return 0U;
}

//--------------------------------------------------------------------------------------------------

uint32_t DriverSAM3X8E::Release()
{
  return 0U;
}

//--------------------------------------------------------------------------------------------------

uint32_t DriverSAM3X8E::Poll()
{
  return 0U;
}

//--------------------------------------------------------------------------------------------------

void DriverSAM3X8E::EndpointXtract(
  uint32_t conf,
  uint32_t iface,
  uint32_t alt,
  uint32_t proto,
  const USB_ENDPOINT_DESCRIPTOR *ep
)
{

}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
