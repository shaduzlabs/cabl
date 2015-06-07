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

//----------------------------------------------------------------------------------------------------------------------

namespace
{

uint16_t kHIDAPIInputBufferSize = 512; // Size of the HIDAPI input buffer
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{

DriverHIDAPI::DriverHIDAPI()
  : m_pCurrentDevice(nullptr)
{
  hid_init();
  m_inputBuffer.resize(kHIDAPIInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DriverHIDAPI::~DriverHIDAPI()
{
  disconnect();
  hid_exit();
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverHIDAPI::connect(Driver::tVendorId vid_, Driver::tProductId pid_)
{

  disconnect();
  m_pCurrentDevice = hid_open(vid_, pid_, NULL);
  if (m_pCurrentDevice == NULL)
    return false;
  hid_set_nonblocking(m_pCurrentDevice, 0);
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DriverHIDAPI::disconnect()
{
  if (m_pCurrentDevice != nullptr)
  {
    hid_close(m_pCurrentDevice);
    m_pCurrentDevice = nullptr;
  }
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverHIDAPI::read(Transfer& transfer_, uint8_t endpoint_)
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

bool DriverHIDAPI::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  if (transfer_)
  {
    int nBytesWritten = hid_write(m_pCurrentDevice, transfer_.getData().data(), transfer_.size());
    return (nBytesWritten >= transfer_.size());
  }

  return false;
}

//----------------------------------------------------------------------------------------------------------------------

} // sl
