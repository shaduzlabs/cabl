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
namespace cabl
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

} // cabl
} // sl
