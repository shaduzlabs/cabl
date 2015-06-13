/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%

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

#include "DriverHIDAPI.h"

namespace
{
  uint16_t kHIDAPIInputBufferSize = 512; // Size of the HIDAPI input buffer
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

DriverHIDAPI::DriverHIDAPI()
{
  hid_init();
}

//----------------------------------------------------------------------------------------------------------------------

DriverHIDAPI::~DriverHIDAPI()
{
  hid_exit();
}

//----------------------------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverHIDAPI::connect(Driver::tVendorId vid_, Driver::tProductId pid_)
{

  tDeviceHandle* pCurrentDevice = hid_open(vid_, pid_, nullptr);
  if (pCurrentDevice == nullptr)
    return nullptr;
    
  hid_set_nonblocking(pCurrentDevice, 0);
  
  return tPtr<DeviceHandleImpl>(new DeviceHandleHIDAPI(pCurrentDevice));
}

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleHIDAPI::DeviceHandleHIDAPI(tDeviceHandle* pDeviceHandle)
  : m_pCurrentDevice(pDeviceHandle)
{
  m_inputBuffer.resize(kHIDAPIInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceHandleHIDAPI::disconnect()
{
  if (m_pCurrentDevice != nullptr)
  {
    hid_close(m_pCurrentDevice);
    m_pCurrentDevice = nullptr;
  }
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleHIDAPI::read(Transfer& transfer_, uint8_t endpoint_)
{
  int nBytesRead = hid_read(m_pCurrentDevice, m_inputBuffer.data(), kHIDAPIInputBufferSize);
  if (nBytesRead > 0)
  {
    transfer_.setData(m_inputBuffer.data(), nBytesRead);
    return transfer_;
  }

  return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleHIDAPI::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  if (transfer_)
  {
    int nBytesWritten = hid_write(m_pCurrentDevice, transfer_.getData().data(), transfer_.size());
    return (nBytesWritten >= static_cast<int>(transfer_.size()));
  }

  return false;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
