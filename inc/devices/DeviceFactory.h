/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceDescriptor.h"
#include "util/Types.h"
#include <functional>
#include <map>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

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
  using tFnCreate = std::function<std::shared_ptr<Device>(void)>;

  static DeviceFactory& instance()
  {
    static DeviceFactory instance;
    return instance;
  }

  std::shared_ptr<Device> device(const DeviceDescriptor&, tPtr<DeviceHandle>);
  bool isKnownDevice(const DeviceDescriptor&) const;

  void registerClass(const DeviceDescriptor&, tFnCreate);

private:
  DeviceFactory() = default;

  std::map<DeviceDescriptor, tFnCreate> m_registry;
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
