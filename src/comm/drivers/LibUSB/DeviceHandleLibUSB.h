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

#include <libusb.h>


namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleLibUSB : public DeviceHandleImpl
{
public:

  DeviceHandleLibUSB(libusb_device_handle*);
  ~DeviceHandleLibUSB();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

  void readAsync(uint8_t endpoint_, DeviceHandle::tCbRead) override;

  static constexpr unsigned kInputBufferSize{512};
  
private:

  void readAsyncImpl(uint8_t endpoint_);
  static void __stdcall cbTransfer(libusb_transfer*);

  std::array<uint8_t, kInputBufferSize> m_inputBuffer;
  libusb_device_handle*                 m_pCurrentDevice;
  
  DeviceHandle::tCbRead                 m_cbRead;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
