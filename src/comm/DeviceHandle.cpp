/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/DeviceHandle.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------
 
DeviceHandle::DeviceHandle( tPtr<DeviceHandleImpl> pDeviceHandle_ )
  :m_pImpl(std::move(pDeviceHandle_))
{

}

//--------------------------------------------------------------------------------------------------

void DeviceHandle::disconnect()
{
  m_pImpl->disconnect();
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandle::read( Transfer& transfer_, uint8_t endpoint_ )
{
  return m_pImpl->read( transfer_, endpoint_ );
}
  
//--------------------------------------------------------------------------------------------------

bool DeviceHandle::write(const Transfer& transfer_, uint8_t endpoint_ )
{
  return m_pImpl->write( transfer_, endpoint_ );
}

//--------------------------------------------------------------------------------------------------

void DeviceHandle::readAsync(uint8_t endpoint_, DeviceHandleImpl::tCbRead cbRead_)
{
  m_pImpl->readAsync(endpoint_, cbRead_);
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
