/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceHandleHIDAPI.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DeviceHandleHIDAPI::DeviceHandleHIDAPI(hid_device* pCurrentDevice_)
  : m_pCurrentDevice(pCurrentDevice_)
{
}

//--------------------------------------------------------------------------------------------------

DeviceHandleHIDAPI::~DeviceHandleHIDAPI()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleHIDAPI::disconnect()
{
  if (m_pCurrentDevice != nullptr)
  {
    hid_close(m_pCurrentDevice);
    m_pCurrentDevice = nullptr;
  }
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleHIDAPI::read(Transfer& transfer_, uint8_t)
{
  int nBytesRead = hid_read(m_pCurrentDevice, m_inputBuffer.data(), kInputBufferSize);

  if (nBytesRead == 0)
  {
    // No data available
    return true;
  }
  else if (nBytesRead > 0)
  {
    transfer_.setData(m_inputBuffer.data(), nBytesRead);
    return transfer_;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleHIDAPI::write(const Transfer& transfer_, uint8_t)
{
  if (transfer_)
  {
    int nBytesWritten = hid_write(m_pCurrentDevice, transfer_.data().data(), transfer_.size());
    return (nBytesWritten >= static_cast<int>(transfer_.size()));
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
