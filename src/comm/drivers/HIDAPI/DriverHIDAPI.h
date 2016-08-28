/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

#include <hidapi.h>

namespace sl
{
namespace cabl
{

class DriverHIDAPI : public DriverImpl
{
public:
  DriverHIDAPI();
  ~DriverHIDAPI() override;

  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl> connect(const DeviceDescriptor&) override;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
