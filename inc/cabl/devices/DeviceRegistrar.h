/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/devices/DeviceFactory.h"

//--------------------------------------------------------------------------------------------------
// Use this macro in the header file of a new device class, after the class declaration
#define M_REGISTER_DEVICE_CLASS(clsName, name, type, vid, pid) \
                                                               \
  static DeviceRegistrar<clsName> registrar_##clsName          \
    = DeviceRegistrar<clsName>({name, type, vid, pid})

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Device;

//--------------------------------------------------------------------------------------------------

template <class T>
class DeviceRegistrar
{
public:
  explicit DeviceRegistrar(const DeviceDescriptor& deviceDescriptor_)
  {
    // Register the factory function for a specific device descriptor
    DeviceFactory::instance().registerClass(deviceDescriptor_,
      [](void) -> std::shared_ptr<Device> { return std::shared_ptr<Device>(new T); });
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
