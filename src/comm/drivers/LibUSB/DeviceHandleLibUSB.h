/*----------------------------------------------------------------------------------------------------------------------   

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <libusb.h>


namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

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
  static void __stdcall cbTransfer(libusb_transfer*); //\todo #define WINAPI      __stdcall

  tRawData                        m_inputBuffer;
  tDeviceHandle*                  m_pCurrentDevice;
  DeviceHandleImpl::tCbRead       m_cbRead;
};
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
