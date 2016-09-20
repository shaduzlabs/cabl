/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>
#include <string>

#include "cabl/util/Log.h"
#include "cabl/util/Types.h"

#include "DeviceDescriptor.h"
#include "DeviceHandle.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DriverImpl;
class Transfer;

//--------------------------------------------------------------------------------------------------

/**
  \class Driver
  \brief The Driver wrapper class

*/

class Driver final
{

public:
  enum class Type : uint8_t
  {
    Probe,
    HIDAPI,
    LibUSB,
    SAM3X8E,
    MAX3421E,
    MIDI,
  };

  using tCollDeviceDescriptor = std::vector<DeviceDescriptor>;
  using tCbHotplug = std::function<void(const DeviceDescriptor&, bool)>;

  explicit Driver(Type type_);

  tCollDeviceDescriptor enumerate();
  tPtr<DeviceHandle> connect(const DeviceDescriptor&);
  void setHotplugCallback(tCbHotplug);

private:
  tPtr<DriverImpl> m_pImpl;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
