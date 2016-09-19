/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

#include <atomic>
#include <thread>

#pragma warning( push )
#pragma warning( disable: 4200 )
#include <libusb.h>
#pragma warning( pop )

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DriverLibUSB : public DriverImpl
{
public:
  DriverLibUSB();
  ~DriverLibUSB() override;

  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl> connect(const DeviceDescriptor&) override;

  void setHotplugCallback(Driver::tCbHotplug) override;

  void hotplug(const DeviceDescriptor&, bool);

private:
  std::atomic<bool> m_usbThreadRunning;
  libusb_hotplug_callback_handle* m_pHotplugHandle{nullptr};

  std::thread m_usbThread;
  libusb_context* m_pContext;
  Driver::tCbHotplug m_cbHotplug;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
