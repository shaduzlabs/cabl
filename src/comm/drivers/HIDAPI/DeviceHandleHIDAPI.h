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

#include <hidapi.h>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleHIDAPI : public DeviceHandleImpl
{
public:
  DeviceHandleHIDAPI(hid_device*);
  ~DeviceHandleHIDAPI();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

  static constexpr unsigned kInputBufferSize{512};

private:
  std::array<uint8_t, kInputBufferSize> m_inputBuffer;
  hid_device* m_pCurrentDevice;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
