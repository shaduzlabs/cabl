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

#pragma once

#include <string>
#include "Device.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class DeviceDescriptor
{
public:
  using tVendorId = uint16_t;
  using tProductId = uint16_t;
  using tSerialNumber = std::string;
  
  DeviceDescriptor(
    tVendorId vendorId_,
    tProductId productId_,
    Device::Type type_,
    tSerialNumber serialNumber_="",
    bool isHid_ = false
  )
    :m_vendorId(vendorId_)
    ,m_productId(productId_)
    ,m_type(type_)
    ,m_serialNumber(serialNumber_)
    ,m_isHid(isHid_)
  { }
  
  tVendorId getVendorId() const{ return m_vendorId; }
  tProductId getProductId() const{ return m_productId; }
  Device::Type getType() const { return m_type; }
  tSerialNumber getSerialNumber() const{ return m_serialNumber; }
  bool isHID() const { return m_isHid; }

  bool operator==(const DeviceDescriptor& other_) const
  {
    return (m_vendorId == other_.m_vendorId)         &&
           (m_productId == other_.m_productId)       &&
           (m_serialNumber == other_.m_serialNumber) &&
           (m_isHid == other_.m_isHid);
  }

  bool operator!=(const DeviceDescriptor& other_) const { return !(operator==(other_)); }
  
  bool isSameProduct(const DeviceDescriptor& other_) const
  {
    return (m_vendorId == other_.m_vendorId) && (m_productId == other_.m_productId);
  }
  
private:

  tVendorId m_vendorId;
  tProductId m_productId;
  Device::Type m_type;
  tSerialNumber m_serialNumber;
  bool m_isHid = false;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
