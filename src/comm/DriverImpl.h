/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cabl/comm/Driver.h>
#include <cabl/comm/Transfer.h>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleImpl;

//--------------------------------------------------------------------------------------------------

class DriverImpl
{

public:
  virtual ~DriverImpl() = default;

  virtual Driver::tCollDeviceDescriptor enumerate() = 0;
  virtual tPtr<DeviceHandleImpl> connect(const DeviceDescriptor&) = 0;
  virtual void setHotplugCallback(Driver::tCbHotplug){};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
