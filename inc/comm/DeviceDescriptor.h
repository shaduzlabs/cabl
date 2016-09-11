/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <iostream>
#include <string>

namespace sl
{
namespace cabl
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
    MIDI,
    Unknown,
  };

  DeviceDescriptor(std::string name_,
    Type type_,
    tVendorId vendorId_,
    tProductId productId_,
    tSerialNumber serialNumber_ = "",
    unsigned portIdIn_ = 0,
    unsigned portIdOut_ = 0)
    : m_name(name_)
    , m_type(type_)
    , m_vendorId(vendorId_)
    , m_productId(productId_)
    , m_serialNumber(serialNumber_)
    , m_portIdIn(portIdIn_)
    , m_portIdOut(portIdOut_)
    , m_local(false)
  {
  }

  const std::string& name() const
  {
    return m_name;
  }
  Type type() const
  {
    return m_type;
  }
  tVendorId vendorId() const
  {
    return m_vendorId;
  }
  tProductId productId() const
  {
    return m_productId;
  }
  unsigned portIdIn() const
  {
    return m_portIdIn;
  }
  unsigned portIdOut() const
  {
    return m_portIdOut;
  }
  const tSerialNumber& serialNumber() const
  {
    return m_serialNumber;
  }
  bool local() const
  {
    return m_local;
  }

  void setLocal(bool local_)
  {
    m_local = local_;
  }

  bool operator==(const DeviceDescriptor& other_) const
  {
    return (m_name == other_.m_name) && (m_type == other_.m_type)
           && (m_vendorId == other_.m_vendorId) && (m_productId == other_.m_productId)
           && (m_serialNumber == other_.m_serialNumber) && (m_portIdIn == other_.m_portIdIn)
           && (m_portIdOut == other_.m_portIdOut);
  }

  bool operator!=(const DeviceDescriptor& other_) const
  {
    return !(operator==(other_));
  }

  bool operator<(const DeviceDescriptor& other_) const
  {
    return (m_name < other_.m_name) || ((m_name == other_.m_name) && (m_type < other_.m_type))
           || ((m_name == other_.m_name) && (m_type == other_.m_type)
                && (m_vendorId < other_.m_vendorId))
           || ((m_name == other_.m_name) && (m_type == other_.m_type)
                && (m_vendorId == other_.m_vendorId)
                && (m_productId < other_.m_productId))
           || ((m_name == other_.m_name) && (m_type == other_.m_type)
                && (m_vendorId == other_.m_vendorId)
                && (m_productId < other_.m_productId)
                && (m_serialNumber < other_.m_serialNumber));
  }
  operator bool() const
  {
    return (m_type != Type::Unknown);
  }

  bool isSameProduct(const DeviceDescriptor& other_) const
  {
    return (m_vendorId == other_.m_vendorId) && (m_productId == other_.m_productId)
           && (m_type == other_.m_type);
  }

  static std::string toString(Type type_)
  {
#define M_TYPE_CASE(item) \
  case Type::item:        \
    return #item
    switch (type_)
    {
      M_TYPE_CASE(USB);
      M_TYPE_CASE(HID);
      M_TYPE_CASE(MIDI);
      M_TYPE_CASE(Unknown);
      default:
      {
        return "Unknown";
      }
    }
#undef M_TYPE_CASE
  }

private:
  friend std::ostream& operator<<(std::ostream& out_, const DeviceDescriptor& dd_)
  {
    out_ << dd_.m_name << "[" << DeviceDescriptor::toString(dd_.m_type) << ":" << std::hex
         << dd_.m_vendorId << std::dec << ":" << std::hex << dd_.m_productId << std::dec << ":"
         << dd_.m_serialNumber << "]";
    return out_;
  }

  std::string m_name;
  Type m_type{Type::Unknown};
  tVendorId m_vendorId;
  tProductId m_productId;
  tSerialNumber m_serialNumber;
  unsigned m_portIdIn;
  unsigned m_portIdOut;
  bool m_local{false};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
