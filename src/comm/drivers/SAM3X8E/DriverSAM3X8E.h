/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <Usb.h>

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DriverSAM3X8E : public DriverImpl, public UsbConfigXtracter
{
public:
  DriverSAM3X8E();
  ~DriverSAM3X8E() override;

  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl> connect(const DeviceDescriptor&) override;

private:
  std::shared_ptr<USBHost> m_pUsb;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
