/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleMAX3421E : public DeviceHandleImpl
{
public:
  using tDeviceHandle = void;

  DeviceHandleMAX3421E(tDeviceHandle*);
  ~DeviceHandleMAX3421E();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

private:
  tRawData m_inputBuffer;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
