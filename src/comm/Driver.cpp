/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/comm/Driver.h"
#include "cabl/comm/DeviceHandle.h"
#include "comm/DriverImpl.h"

#include "comm/drivers/Probe/DriverProbe.h"

#if defined(__SAM3X8E__)
#include "comm/drivers/SAM3X8E/DriverSAM3X8E.h"
#elif defined(__MAX3421E__)
#include "comm/drivers/MAX3421E/DriverMAX3421E.h"
#else
#include "comm/drivers/HIDAPI/DriverHIDAPI.h"
#include "comm/drivers/LibUSB/DriverLibUSB.h"
#include "comm/drivers/MIDI/DriverMIDI.h"
#endif

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

Driver::Driver(Type type_)
{
  switch (type_)
  {
#if defined(__SAM3X8E__)
    case Type::SAM3X8E:
      m_pImpl.reset(new DriverSAM3X8E);
      break;
#elif defined(__MAX3421E__)
    case Type::MAX3421E:
      m_pImpl.reset(new DriverMAX3421E);
      break;
#else
    case Type::HIDAPI:
      m_pImpl.reset(new DriverHIDAPI);
      break;
    case Type::LibUSB:
      m_pImpl.reset(new DriverLibUSB);
      break;
    case Type::MIDI:
      m_pImpl.reset(new DriverMIDI);
      break;
#endif
    case Type::Probe:
    default:
      m_pImpl.reset(new DriverProbe);
      break;
  }
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor Driver::enumerate()
{
  return m_pImpl->enumerate();
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandle> Driver::connect(const DeviceDescriptor& device_)
{
  return tPtr<DeviceHandle>(new DeviceHandle(m_pImpl->connect(device_)));
}

//--------------------------------------------------------------------------------------------------

void Driver::setHotplugCallback(Driver::tCbHotplug cbHotplug_)
{
  m_pImpl->setHotplugCallback(cbHotplug_);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
