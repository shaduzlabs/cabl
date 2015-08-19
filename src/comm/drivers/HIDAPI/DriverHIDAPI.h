/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <hidapi.h>

namespace sl
{
namespace kio
{

class DriverHIDAPI : public DriverImpl
{
public:
  
  using tDeviceHandle = hid_device;  
  
  DriverHIDAPI();
  ~DriverHIDAPI() override;
  
  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl>        connect(const DeviceDescriptor&) override;
  
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
