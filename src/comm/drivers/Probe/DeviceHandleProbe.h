/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleProbe : public DeviceHandleImpl
{
public:
  ~DeviceHandleProbe();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

private:
  static uint32_t s_numPacketR;
  static uint32_t s_numPacketW;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
