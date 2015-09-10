/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <Usb.h>
#include <confdescparser.h>

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DriverSAM3X8E : public DriverImpl, public UsbConfigXtracter, public USBDeviceConfig
{
public:

  DriverSAM3X8E();
  ~DriverSAM3X8E() override;

  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl>        connect(const DeviceDescriptor&) override;

private:

  uint32_t Init(uint32_t parent, uint32_t port, uint32_t lowspeed) override;

  uint32_t Release() override;

  uint32_t Poll() override;

  uint32_t GetAddress() override { return 0/*m_deviceAddress*/; };

  void EndpointXtract(
    uint32_t conf,
    uint32_t iface,
    uint32_t alt,
    uint32_t proto,
    const USB_ENDPOINT_DESCRIPTOR *ep
  ) override;

  std::shared_ptr<USBHost>      m_pUsb;

};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
