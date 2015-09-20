/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <hidapi.h>

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleHIDAPI : public DeviceHandleImpl
{
public:

  DeviceHandleHIDAPI(hid_device*);
  ~DeviceHandleHIDAPI();

  void disconnect() override;

  bool read( Transfer&, uint8_t ) override;
  bool write( const Transfer& , uint8_t) override;

private:

  tRawData                        m_inputBuffer;
  hid_device*                     m_pCurrentDevice;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
