/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/nanomsg/DriverNanomsg.h"
#include "comm/drivers/nanomsg/DeviceHandleNanomsg.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DriverNanomsg::DriverNanomsg()
{
}

//--------------------------------------------------------------------------------------------------

DriverNanomsg::~DriverNanomsg()
{
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverNanomsg::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverNanomsg::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
