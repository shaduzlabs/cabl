/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceHandleLibUSB.h"

namespace
{
  uint16_t kLibUSBInputBufferSize = 512;  // Size of the LIBUSB input buffer
  uint16_t kLibUSBReadTimeout =  2;       // Timeout of a input bulk transfer (0 = NO timeout)
  uint16_t kLibUSBWriteTimeout = 50;      // Timeout of a output bulk transfer (0 = NO timeout)
}

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------

DeviceHandleLibUSB::DeviceHandleLibUSB(tDeviceHandle* pDeviceHandle)
  : m_pCurrentDevice(pDeviceHandle)
{
  m_inputBuffer.resize(kLibUSBInputBufferSize);
}

//--------------------------------------------------------------------------------------------------

DeviceHandleLibUSB::~DeviceHandleLibUSB()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleLibUSB::disconnect()
{
  if( m_pCurrentDevice != nullptr )
  {
    libusb_close( m_pCurrentDevice );
    m_pCurrentDevice = nullptr;
  }
}
  
//--------------------------------------------------------------------------------------------------
  
bool DeviceHandleLibUSB::read( Transfer& transfer_, uint8_t endpoint_ )
{
  int nBytesRead = 0;
  int result = libusb_bulk_transfer(
    m_pCurrentDevice,                 // Device handle
    endpoint_,                        // Endpoint
    m_inputBuffer.data(),             // Data pointer
    kLibUSBInputBufferSize,           // Size of data
    &nBytesRead,                      // N. of bytes actually read
    kLibUSBReadTimeout                // Timeout
  );
  
  if( ( LIBUSB_SUCCESS == result ) && ( nBytesRead > 0 ) )
  {
    transfer_.setData( m_inputBuffer.data(), nBytesRead );
    return transfer_;
  }
  
  return false;
}
  
//--------------------------------------------------------------------------------------------------
  
bool DeviceHandleLibUSB::write( const Transfer& transfer_, uint8_t endpoint_ ) const
{
  int nBytesWritten = 0;
  if( transfer_ == true )
  {
    int result = libusb_bulk_transfer(
      m_pCurrentDevice,                 // Device handle
      endpoint_,                        // Endpoint
      const_cast<uint8_t*>(transfer_.getData().data()),       // Data pointer
      transfer_.size(),                 // Size of data
      &nBytesWritten,                   // N. of bytes actually written
      kLibUSBWriteTimeout                     // Timeout
    );
    if(( LIBUSB_SUCCESS != result ) || ( nBytesWritten != transfer_.size() ))
    {
      M_LOG("[DeviceHandleLibUSB] write: error=" << result << " - transfer size: "
            << transfer_.size() << " written: " << nBytesWritten);
    }
    return( ( LIBUSB_SUCCESS == result ) && ( nBytesWritten == transfer_.size() ) );
  }
  
  return false;
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleLibUSB::readAsync(uint8_t endpoint_, DeviceHandleImpl::tCbRead cbRead_)
{
  m_cbRead = cbRead_;
  readAsyncImpl(endpoint_);
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleLibUSB::readAsyncImpl(uint8_t endpoint_)
{
  libusb_transfer* pTransfer = libusb_alloc_transfer(0);
  libusb_fill_bulk_transfer(
    pTransfer,
    m_pCurrentDevice,
    endpoint_,
    m_inputBuffer.data(),
    kLibUSBInputBufferSize,
    cbTransfer,
    this,
    kLibUSBReadTimeout
    );
  libusb_submit_transfer(pTransfer);
  //!\todo check libusb_submit_transfer return code
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleLibUSB::cbTransfer(libusb_transfer* pTransfer_)
{
  DeviceHandleLibUSB* pSelf = (DeviceHandleLibUSB*)pTransfer_->user_data;
  if(
    pSelf->m_cbRead && 
    pTransfer_->status == LIBUSB_TRANSFER_COMPLETED && 
    pTransfer_->actual_length > 0
  )
  {
    tRawData data(pTransfer_->buffer, pTransfer_->buffer + pTransfer_->actual_length);
    pSelf->m_cbRead({ data });
  }
  if (pSelf->m_pCurrentDevice)
  {
    pSelf->readAsyncImpl(pTransfer_->endpoint);
  }
}
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
