/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <libusb.h>


namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleLibUSB : public DeviceHandleImpl
{
public:

  using tDeviceHandle = struct ::libusb_device_handle;

  DeviceHandleLibUSB(tDeviceHandle*);
  ~DeviceHandleLibUSB();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) const override;

  void readAsync(uint8_t endpoint_, DeviceHandleImpl::tCbRead) override;

private:

  void readAsyncImpl(uint8_t endpoint_);
  static void __stdcall cbTransfer(libusb_transfer*); //!\todo #define WINAPI      __stdcall

  tRawData                        m_inputBuffer;
  tDeviceHandle*                  m_pCurrentDevice;
  DeviceHandleImpl::tCbRead       m_cbRead;
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
