/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverHIDAPI.h"
#include "DeviceHandleHIDAPI.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DriverHIDAPI::DriverHIDAPI()
{
  int res = hid_init();
  M_LOG("[HIDAPI] initialization (" << res << ")");
}

//--------------------------------------------------------------------------------------------------

DriverHIDAPI::~DriverHIDAPI()
{
  int res = hid_exit();
  M_LOG("[HIDAPI] exit (" << res << ")");
}
  
//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverHIDAPI::enumerate()
{
  M_LOG("[HIDAPI] enumerate");
  Driver::tCollDeviceDescriptor collDeviceDescriptor;
  struct hid_device_info *devices;

	devices = hid_enumerate(0x0, 0x0);
	while (devices)
  {
    std::string strSerialNumber;
    if(devices->serial_number !=nullptr)
    {
      std::wstring wSerialNumber(devices->serial_number);
      strSerialNumber = std::string(wSerialNumber.begin(), wSerialNumber.end());
    }

    DeviceDescriptor deviceDescriptor(
      devices->vendor_id,
      devices->product_id,
      strSerialNumber,
      true
    );
    M_LOG("[HIDAPI] enumerate: found " << devices->product_string  
       << " with S/N = " << strSerialNumber);

    collDeviceDescriptor.push_back(deviceDescriptor);
		devices = devices->next;
	}
	hid_free_enumeration(devices);
  
  return collDeviceDescriptor;
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverHIDAPI::connect(const DeviceDescriptor& device_)
{
  M_LOG("[HIDAPI] connecting to " << device_.getVendorId() << ":"
    << device_.getProductId() << ":"
    << device_.getSerialNumber());

  std::string serialNumber(device_.getSerialNumber());
  std::wstring wSerialNumber(serialNumber.begin(), serialNumber.end());

  hid_device* pCurrentDevice = hid_open(
    device_.getVendorId(), 
    device_.getProductId(), 
    wSerialNumber.c_str()
  );
  
  if (pCurrentDevice == nullptr)
    return nullptr;
    
  hid_set_nonblocking(pCurrentDevice, 1);
  
  M_LOG("[HIDAPI] CONNECTED to " << device_.getVendorId() << ":"
    << device_.getProductId() << ":"
    << device_.getSerialNumber());

  return tPtr<DeviceHandleImpl>(new DeviceHandleHIDAPI(pCurrentDevice));
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
