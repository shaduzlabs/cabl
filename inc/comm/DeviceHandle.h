/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>
#include "util/Types.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class Transfer;

//--------------------------------------------------------------------------------------------------

class DeviceHandle
{

public:

  DeviceHandle(tPtr<DeviceHandleImpl>);

  void disconnect();

  bool read(Transfer&, uint8_t);
  bool write(const Transfer&, uint8_t) const;

  void readAsync(uint8_t, DeviceHandleImpl::tCbRead);

private:
  tPtr<DeviceHandleImpl> m_pImpl;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
