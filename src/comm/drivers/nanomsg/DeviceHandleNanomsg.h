/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleNanomsg : public DeviceHandleImpl
{
public:

  ~DeviceHandleNanomsg();
  
  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

private:

  static uint32_t    s_numPacketR;
  static uint32_t    s_numPacketW;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
