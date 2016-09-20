/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/comm/DeviceHandle.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DeviceHandle::DeviceHandle(tPtr<DeviceHandleImpl> pDeviceHandle_)
  : m_pImpl(std::move(pDeviceHandle_))
{
}

//--------------------------------------------------------------------------------------------------

DeviceHandle::~DeviceHandle()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandle::disconnect()
{
  m_pImpl->disconnect();
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandle::read(Transfer& transfer_, uint8_t endpoint_)
{
  return m_pImpl->read(transfer_, endpoint_);
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandle::write(const Transfer& transfer_, uint8_t endpoint_)
{
  return m_pImpl->write(transfer_, endpoint_);
}

//--------------------------------------------------------------------------------------------------

void DeviceHandle::readAsync(uint8_t endpoint_, DeviceHandle::tCbRead cbRead_)
{
  m_pImpl->readAsync(endpoint_, cbRead_);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
