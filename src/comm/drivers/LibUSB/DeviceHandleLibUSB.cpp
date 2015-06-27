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

#include "DeviceHandleLibUSB.h"

namespace
{
  uint16_t kLibUSBInputBufferSize = 512;  // Size of the LIBUSB input buffer
  uint16_t kLibUSBReadTimeout =  10;            // Timeout of a input bulk transfer (0 = NO timeout)
  uint16_t kLibUSBWriteTimeout = 60;            // Timeout of a output bulk transfer (0 = NO timeout)
}

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

DeviceHandleLibUSB::DeviceHandleLibUSB(tDeviceHandle* pDeviceHandle)
  : m_pCurrentDevice(pDeviceHandle)
{
  m_inputBuffer.resize(kLibUSBInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleLibUSB::~DeviceHandleLibUSB()
{
  disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceHandleLibUSB::disconnect()
{
  if( m_pCurrentDevice != nullptr )
  {
    libusb_close( m_pCurrentDevice );
    m_pCurrentDevice = nullptr;
  }
}
  
//----------------------------------------------------------------------------------------------------------------------
  
bool DeviceHandleLibUSB::read( Transfer& transfer_, uint8_t endpoint_ )
{
  int nBytesRead = 0;
  int result = libusb_bulk_transfer(
    m_pCurrentDevice,                 // Device handle
    endpoint_,                        // Endpoint
    m_inputBuffer.data(),             // Data pointer
    kLibUSBInputBufferSize,           // Size of data
    &nBytesRead,                      // N. of bytes actually written
    kLibUSBReadTimeout                // Timeout
  );
  
  if( ( LIBUSB_SUCCESS == result ) && ( nBytesRead > 0 ) )
  {
    transfer_.setData( m_inputBuffer.data(), nBytesRead );
    return transfer_;
  }
 
  return false;
}
  
//----------------------------------------------------------------------------------------------------------------------
  
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
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
