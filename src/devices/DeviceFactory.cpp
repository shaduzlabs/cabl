/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

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

#include "devices/DeviceFactory.h"

#include "devices/Device.h"
#include "comm/DeviceDescriptor.h"

#include "devices/DeviceMaschineMK1.h"
#include "devices/DeviceMaschineMK2.h"
#include "devices/DeviceMaschineMikroMK2.h"

//----------------------------------------------------------------------------------------------------------------------

namespace
{
static const unsigned kVendor_NativeInstruments = 0x17CC;

static const unsigned kProduct_MaschineMK1      = 0x0808;
static const unsigned kProduct_MaschineMK2      = 0x1140;
static const unsigned kProduct_MaschineMikroMK1 = 0x1110;
static const unsigned kProduct_MaschineMikroMK2 = 0x1200;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

Device* DeviceFactory::getDevice(const DeviceDescriptor& deviceDescriptor_, tPtr<DeviceHandle> pDeviceHandle_)
{
  if(deviceDescriptor_.getVendorId()!=kVendor_NativeInstruments)
  {
    return nullptr;
  }
  switch(deviceDescriptor_.getProductId())
  {
    case kProduct_MaschineMK1:
    {
      return new DeviceMaschineMK1(std::move(pDeviceHandle_));
    }
    case kProduct_MaschineMK2:
    {
      return new DeviceMaschineMK2(std::move(pDeviceHandle_));
    }
    case kProduct_MaschineMikroMK2:
    {
      return new DeviceMaschineMikroMK2(std::move(pDeviceHandle_));
    }
    default:
      return nullptr;
  }
  return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
