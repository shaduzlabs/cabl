/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <thread>

#include <libusb.h>

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------

class DriverLibUSB : public DriverImpl
{
public:

  using tDeviceHandle   = struct ::libusb_device_handle; 
  
  DriverLibUSB();
  ~DriverLibUSB() override;
  
  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl>        connect(const DeviceDescriptor&) override;

private:

  std::string getStringDescriptor(tDeviceHandle*, uint8_t);
  bool                            m_usbThreadRunning;
  std::thread                     m_usbThread;
  libusb_context*                 m_pContext;
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
