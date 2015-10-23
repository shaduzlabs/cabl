/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <comm/Driver.h>
#include <comm/Transfer.h>

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

  virtual Driver::tCollDeviceDescriptor  enumerate() = 0;
  virtual tPtr<DeviceHandleImpl>         connect( const DeviceDescriptor& ) = 0;

};
  
//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
