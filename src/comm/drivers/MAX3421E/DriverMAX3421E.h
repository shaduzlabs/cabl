/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DriverMAX3421E : public DriverImpl
{
public:

  DriverMAX3421E();
  ~DriverMAX3421E() override;
  
  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl>        connect(const DeviceDescriptor&) override;

};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
