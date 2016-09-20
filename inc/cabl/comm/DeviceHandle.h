/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>
#include <functional>

#include "cabl/util/Types.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Transfer;
class DeviceHandleImpl;

//--------------------------------------------------------------------------------------------------

class DeviceHandle
{

public:
  using tCbRead = std::function<void(Transfer)>;

  explicit DeviceHandle(tPtr<DeviceHandleImpl>);
  ~DeviceHandle();

  void disconnect();

  bool read(Transfer&, uint8_t);
  bool write(const Transfer&, uint8_t);

  void readAsync(uint8_t, tCbRead);

private:
  tPtr<DeviceHandleImpl> m_pImpl;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
