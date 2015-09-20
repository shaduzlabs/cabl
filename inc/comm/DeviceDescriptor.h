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
  
  enum class Type
  {
    USB,
    HID,
    MIDI
  };
  
  DeviceDescriptor(
    std::string name_,
    Type type_,
    tVendorId vendorId_,
    tProductId productId_,
    tSerialNumber serialNumber_="",
    unsigned portIdIn_ = 0,
    unsigned portIdOut_ = 0
  )
    :m_name(name_)
    ,m_type(type_)
    ,m_vendorId(vendorId_)
    ,m_productId(productId_)
    ,m_serialNumber(serialNumber_)
    ,m_portIdIn(portIdIn_)
    ,m_portIdOut(portIdOut_)
  { }
  
  const std::string& getName() const{ return m_name; }
  Type getType() const { return m_type; }
  tVendorId getVendorId() const{ return m_vendorId; }
  tProductId getProductId() const{ return m_productId; }
  unsigned getPortIdIn() const { return m_portIdIn; }
  unsigned getPortIdOut() const { return m_portIdOut; }
  const tSerialNumber& getSerialNumber() const{ return m_serialNumber; }

  bool operator==(const DeviceDescriptor& other_) const
  {
    return (m_name == other_.m_name)                 &&
//           (m_type == other_.m_type)               &&
           (m_vendorId == other_.m_vendorId)         &&
           (m_productId == other_.m_productId)       &&
           (m_serialNumber == other_.m_serialNumber) &&
           (m_portIdIn == other_.m_portIdIn)         &&
           (m_portIdOut == other_.m_portIdOut);
  }

  bool operator!=(const DeviceDescriptor& other_) const { return !(operator==(other_)); }
  
  bool isSameProduct(const DeviceDescriptor& other_) const
  {
    return (m_name == other_.m_name)           &&
           (m_vendorId == other_.m_vendorId)   &&
           (m_productId == other_.m_productId);
  }
  
private:

  std::string     m_name;
  Type            m_type;
  tVendorId       m_vendorId;
  tProductId      m_productId;
  tSerialNumber   m_serialNumber;
  unsigned        m_portIdIn;
  unsigned        m_portIdOut;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
