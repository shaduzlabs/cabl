/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceHandleHIDAPI.h"

namespace
{
  uint16_t kHIDAPIInputBufferSize = 512; // Size of the HIDAPI input buffer
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DeviceHandleHIDAPI::DeviceHandleHIDAPI(hid_device* pCurrentDevice_)
  : m_pCurrentDevice(pCurrentDevice_)
{
  m_inputBuffer.resize(kHIDAPIInputBufferSize);
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
  int nBytesRead = hid_read(m_pCurrentDevice, m_inputBuffer.data(), kHIDAPIInputBufferSize);

  //!\todo check when hid_read returns 0 (no data available to read) and do not return error!
  if(nBytesRead ==0)
  {
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
    int nBytesWritten = hid_write(m_pCurrentDevice, transfer_.getData().data(), transfer_.size());
    return (nBytesWritten >= static_cast<int>(transfer_.size()));
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
