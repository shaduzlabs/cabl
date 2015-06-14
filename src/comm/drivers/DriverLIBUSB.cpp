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
  uint16_t kLIBUSBReadTimeout =  10;            // Timeout of a input bulk transfer (0 = NO timeout)
  uint16_t kLIBUSBWriteTimeout = 60;            // Timeout of a output bulk transfer (0 = NO timeout)
}

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

DriverLIBUSB::DriverLIBUSB()
{
  libusb_init(&m_pContext);

#ifdef DEBUG
  libusb_set_debug( m_pContext, 3);
#endif
}
  
//----------------------------------------------------------------------------------------------------------------------
  
DriverLIBUSB::~DriverLIBUSB()
{
  libusb_exit( m_pContext );
}
  
//----------------------------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverLIBUSB::enumerate()
{
  Driver::tCollDeviceDescriptor collDeviceDescriptor;
  
  libusb_device **devices;
  ssize_t nDevices = libusb_get_device_list(m_pContext, &devices);
  
  tDeviceHandle* pHandle = nullptr;
  for( int i=0; i<nDevices; ++i )
  {
    libusb_device *device = devices[i];
    if(libusb_open(device, &pHandle) < 0)
    {
      continue;
    }
    struct libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    std::string strSerialNumber;
    if(descriptor.iSerialNumber != 0)
    {
      char sNum[256];
      
      if(libusb_get_string_descriptor_ascii(pHandle, descriptor.iSerialNumber,(unsigned char*)sNum, sizeof(sNum)) > 0)
      {
        strSerialNumber = sNum;
      }
    }
    libusb_close(pHandle);
    DeviceDescriptor deviceDescriptor(
      descriptor.idVendor,
      descriptor.idProduct,
      strSerialNumber,
      false
    );

    collDeviceDescriptor.push_back(deviceDescriptor);
  }
  
  libusb_free_device_list(devices, 1);

  return collDeviceDescriptor;
}

//----------------------------------------------------------------------------------------------------------------------
  
tPtr<DeviceHandleImpl> DriverLIBUSB::connect( const DeviceDescriptor& device_ )
{
  
  bool bConnected = false;
  libusb_device **devices;
  ssize_t nDevices = libusb_get_device_list(m_pContext, &devices);
  
  tDeviceHandle* pCurrentDevice = nullptr;
  for( int i=0; i<nDevices; ++i )
  {
    libusb_device *device = devices[i];
    struct libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    if( (descriptor.idVendor != device_.getVendorId()) ||
        (descriptor.idProduct != device_.getProductId())
    )
    {
      continue;  // ADD SERIAL NUMBER!
    }
    
    int e = libusb_open(device, &pCurrentDevice);
    if(e == 0)
    {
      bConnected = true;
      break;
    }
  }
  
  libusb_free_device_list(devices, 1);
  
  libusb_set_configuration(pCurrentDevice, 1 );
  libusb_claim_interface(pCurrentDevice, 0 );

  libusb_set_interface_alt_setting  (pCurrentDevice, 0, 1 );
  
  if(pCurrentDevice == nullptr || !bConnected)
    return nullptr;

  return tPtr<DeviceHandleImpl>(new DeviceHandleLibUSB(pCurrentDevice));
}

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleLibUSB::DeviceHandleLibUSB(tDeviceHandle* pDeviceHandle)
  : m_pCurrentDevice(pDeviceHandle)
{
  m_inputBuffer.resize(kLIBUSBInputBufferSize);
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
    kLIBUSBInputBufferSize,           // Size of data
    &nBytesRead,                      // N. of bytes actually written
    kLIBUSBReadTimeout                // Timeout
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
      kLIBUSBWriteTimeout                     // Timeout
    );
    return( ( LIBUSB_SUCCESS == result ) && ( nBytesWritten == transfer_.size() ) );
  }
  
  return false;
}
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
