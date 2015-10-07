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

  DeviceHandleLibUSB(libusb_device_handle*);
  ~DeviceHandleLibUSB();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

  void readAsync(uint8_t endpoint_, DeviceHandle::tCbRead) override;

private:

  void readAsyncImpl(uint8_t endpoint_);
  static void __stdcall cbTransfer(libusb_transfer*); //!\todo #define WINAPI      __stdcall

  tRawData                        m_inputBuffer;
  libusb_device_handle*           m_pCurrentDevice;
  
  DeviceHandle::tCbRead           m_cbRead;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
