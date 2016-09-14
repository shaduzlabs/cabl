/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

//--------------------------------------------------------------------------------------------------

#include "cabl/comm/DeviceDescriptor.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DiscoveryPolicy
{

public:
  DiscoveryPolicy(const std::string& name_ = "*",
    DeviceDescriptor::tVendorId vendorId_ = 0,
    DeviceDescriptor::tProductId productId_ = 0,
    DeviceDescriptor::Type type_ = DeviceDescriptor::Type::Unknown)
    : m_name(name_), m_vendorId(vendorId_), m_productId(productId_), m_type(type_)
  {
  }

  const std::string name() const
  {
    return m_name;
  }
  DeviceDescriptor::tVendorId vendorId() const
  {
    return m_vendorId;
  }
  DeviceDescriptor::tProductId productId() const
  {
    return m_productId;
  }
  DeviceDescriptor::Type type() const
  {
    return m_type;
  }

  bool matches(const DeviceDescriptor&);

private:
  std::string m_name{"*"};
  DeviceDescriptor::tVendorId m_vendorId{0};
  DeviceDescriptor::tProductId m_productId{0};
  DeviceDescriptor::Type m_type{DeviceDescriptor::Type::Unknown};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
