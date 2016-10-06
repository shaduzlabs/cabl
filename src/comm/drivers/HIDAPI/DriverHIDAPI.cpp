/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverHIDAPI.h"

#include <tuple>

#include "DeviceHandleHIDAPI.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DriverHIDAPI::DriverHIDAPI()
{
  int res = hid_init();
  std::ignore = res;
  M_LOG("[HIDAPI] initialization (" << res << ")");
}

//--------------------------------------------------------------------------------------------------

DriverHIDAPI::~DriverHIDAPI()
{
  int res = hid_exit();
  std::ignore = res;
  M_LOG("[HIDAPI] exit (" << res << ")");
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverHIDAPI::enumerate()
{
  M_LOG("[HIDAPI] enumerate");
  Driver::tCollDeviceDescriptor collDeviceDescriptor;

  auto devices = hid_enumerate(0x0, 0x0);
  while (devices != nullptr)
  {
    std::string strSerialNumber;
    if (devices->serial_number != nullptr)
    {
      std::wstring wSerialNumber(devices->serial_number);
      strSerialNumber = std::string(wSerialNumber.begin(), wSerialNumber.end());
    }

    std::string strProductName;
    if (devices->product_string != nullptr)
    {
      std::wstring wProductName(devices->product_string);
      strProductName = std::string(wProductName.begin(), wProductName.end());
    }
    DeviceDescriptor deviceDescriptor(strProductName,
      DeviceDescriptor::Type::HID,
      devices->vendor_id,
      devices->product_id,
      strSerialNumber);
    M_LOG("[HIDAPI] enumerate: found " << strProductName << " with S/N = " << strSerialNumber);

    collDeviceDescriptor.push_back(deviceDescriptor);
    devices = devices->next;
  }
  hid_free_enumeration(devices);

  return collDeviceDescriptor;
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverHIDAPI::connect(const DeviceDescriptor& device_)
{
  M_LOG("[HIDAPI] connecting to " << device_.vendorId() << ":" << device_.productId() << ":"
                                  << device_.serialNumber());

  std::string serialNumber(device_.serialNumber());
  std::wstring wSerialNumber(serialNumber.begin(), serialNumber.end());

  hid_device* pCurrentDevice
    = hid_open(device_.vendorId(), device_.productId(), wSerialNumber.c_str());

  if (pCurrentDevice == nullptr)
  {
    return nullptr;
  }
  
  hid_set_nonblocking(pCurrentDevice, 1);

  M_LOG("[HIDAPI] CONNECTED to " << device_.vendorId() << ":" << device_.productId() << ":"
                                 << device_.serialNumber());

  return tPtr<DeviceHandleImpl>(new DeviceHandleHIDAPI(pCurrentDevice));
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
