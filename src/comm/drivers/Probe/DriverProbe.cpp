/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/Probe/DriverProbe.h"
#include "comm/drivers/Probe/DeviceHandleProbe.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DriverProbe::DriverProbe()
{
}

//--------------------------------------------------------------------------------------------------

DriverProbe::~DriverProbe()
{
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverProbe::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverProbe::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
