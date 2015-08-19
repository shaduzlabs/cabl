/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <string>

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceDescriptor
{
public:
  using tVendorId = uint16_t;
  using tProductId = uint16_t;
  using tSerialNumber = std::string;
  
  DeviceDescriptor(
    tVendorId vendorId_,
    tProductId productId_,
    tSerialNumber serialNumber_="",
    bool isHid_ = false
  )
    :m_vendorId(vendorId_)
    ,m_productId(productId_)
    ,m_serialNumber(serialNumber_)
    ,m_isHid(isHid_)
  { }
  
  tVendorId getVendorId() const{ return m_vendorId; }
  tProductId getProductId() const{ return m_productId; }
  tSerialNumber getSerialNumber() const{ return m_serialNumber; }
  bool isHID() const { return m_isHid; }

  bool operator==(const DeviceDescriptor& other_) const
  {
    return (m_vendorId == other_.m_vendorId)         &&
           (m_productId == other_.m_productId)       &&
           (m_serialNumber == other_.m_serialNumber) ;//&&
//           (m_isHid == other_.m_isHid);
  }

  bool operator!=(const DeviceDescriptor& other_) const { return !(operator==(other_)); }
  
  bool isSameProduct(const DeviceDescriptor& other_) const
  {
    return (m_vendorId == other_.m_vendorId) && (m_productId == other_.m_productId);
  }
  
private:

  tVendorId m_vendorId;
  tProductId m_productId;
  tSerialNumber m_serialNumber;
  bool m_isHid = false;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
