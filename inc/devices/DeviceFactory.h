/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "util/Types.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceDescriptor;
class DeviceHandle;

//--------------------------------------------------------------------------------------------------

namespace devices
{

//--------------------------------------------------------------------------------------------------

class Device;

//--------------------------------------------------------------------------------------------------

class DeviceFactory
{
public:

  static Device* getDevice(const DeviceDescriptor&, tPtr<DeviceHandle>);

};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
