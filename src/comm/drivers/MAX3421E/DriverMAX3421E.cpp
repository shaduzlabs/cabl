/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverMAX3421E.h"
#include "DeviceHandleMAX3421E.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DriverMAX3421E::DriverMAX3421E()
{

}

//--------------------------------------------------------------------------------------------------

DriverMAX3421E::~DriverMAX3421E()
{

}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverMAX3421E::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverMAX3421E::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
