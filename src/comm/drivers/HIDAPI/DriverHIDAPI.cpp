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

#include "DriverHIDAPI.h"
#include "DeviceHandleHIDAPI.h"

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

DriverHIDAPI::DriverHIDAPI()
{
  hid_init();
}

//----------------------------------------------------------------------------------------------------------------------

DriverHIDAPI::~DriverHIDAPI()
{
  hid_exit();
}
  
//----------------------------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverHIDAPI::enumerate()
{
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
    collDeviceDescriptor.push_back(deviceDescriptor);
		devices = devices->next;
	}
	hid_free_enumeration(devices);
  
  return collDeviceDescriptor;
}

//----------------------------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverHIDAPI::connect(const DeviceDescriptor& device_)
{
  std::string serialNumber(device_.getSerialNumber());
  std::wstring wSerialNumber(serialNumber.begin(), serialNumber.end());
  tDeviceHandle* pCurrentDevice = hid_open(device_.getVendorId(), device_.getProductId(), wSerialNumber.c_str());
  if (pCurrentDevice == nullptr)
    return nullptr;
    
  hid_set_nonblocking(pCurrentDevice, 0);
  
  return tPtr<DeviceHandleImpl>(new DeviceHandleHIDAPI(pCurrentDevice));
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
