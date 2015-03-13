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

#include "DriverLIBUSB.h"

namespace
{
  
  uint16_t kLIBUSBInputBufferSize = 512;  // Size of the LIBUSB input buffer
  uint16_t kReadTimeout =  10;              // Timeout of a input bulk transfer (0 = NO timeout)
  uint16_t kWriteTimeout = 60;             // Timeout of a output bulk transfer (0 = NO timeout)
  
}

namespace sl
{
  
DriverLIBUSB::DriverLIBUSB()
  : m_inputBuffer( new uint8_t[ kLIBUSBInputBufferSize ] )
  , m_pCurrentDevice(nullptr)
{
  libusb_init( &m_pContext );

#ifdef DEBUG
  libusb_set_debug	( m_pContext, 3 );
#endif

}
  
//----------------------------------------------------------------------------------------------------------------------
  
DriverLIBUSB::~DriverLIBUSB()
{
  disconnect();
  libusb_exit( m_pContext );
}
  
//----------------------------------------------------------------------------------------------------------------------
  
bool DriverLIBUSB::connect( Driver::tVendorId vid_, Driver::tProductId pid_ )
{
  disconnect();
  
  bool bConnected = false;
  libusb_device **devices;
  ssize_t nDevices = libusb_get_device_list(m_pContext, &devices);
  
  for( int i=0; i<nDevices; ++i )
  {
    libusb_device *device = devices[i];
    struct libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    if( ( descriptor.idVendor != vid_ ) || ( descriptor.idProduct != pid_ ) )
      continue;
    
    int e = libusb_open(device, &m_pCurrentDevice);
    if(e == 0)
    {
      bConnected = true;
      break;
    }
  }
  
  libusb_free_device_list(devices, 1);
  
  libusb_set_configuration( m_pCurrentDevice, 1 );
  libusb_claim_interface( m_pCurrentDevice, 0 );

  libusb_set_interface_alt_setting	(	m_pCurrentDevice, 0, 1 );
  
  if( m_pCurrentDevice == NULL)
    return false;

  return bConnected;
}
  
//----------------------------------------------------------------------------------------------------------------------

void DriverLIBUSB::disconnect()
{
  if( m_pCurrentDevice != nullptr )
  {
    libusb_close( m_pCurrentDevice );
    m_pCurrentDevice = nullptr;
  }
}
  
//----------------------------------------------------------------------------------------------------------------------
  
bool DriverLIBUSB::read( Transfer& transfer_, uint8_t endpoint_ ) const
{
  int nBytesRead = 0;
  int result = libusb_bulk_transfer(
    m_pCurrentDevice,                 // Device handle
    endpoint_,                         // Endpoint
    m_inputBuffer.get(),              // Data pointer
    kLIBUSBInputBufferSize,           // Size of data
    &nBytesRead,                      // N. of bytes actually written
    kReadTimeout                      // Timeout
  );
  
  if( ( LIBUSB_SUCCESS == result ) && ( nBytesRead > 0 ) )
  {
    transfer_.setData( m_inputBuffer.get(), nBytesRead );
    return transfer_;
  }
 
  return false;
}
  
//----------------------------------------------------------------------------------------------------------------------
  
bool DriverLIBUSB::write( const Transfer& transfer_, uint8_t endpoint_ ) const
{
  int nBytesWritten = 0;
  if( transfer_ == true )
  {
    int result = libusb_bulk_transfer(
      m_pCurrentDevice,                 // Device handle
      endpoint_,                        // Endpoint
      transfer_.getDataPtr(),           // Data pointer
      transfer_.getSize(),              // Size of data
      &nBytesWritten,                   // N. of bytes actually written
      kWriteTimeout                     // Timeout
    );
    return( ( LIBUSB_SUCCESS == result ) && ( nBytesWritten == transfer_.getSize() ) );
  }
  
  return false;
}
  
//----------------------------------------------------------------------------------------------------------------------

} // sl
