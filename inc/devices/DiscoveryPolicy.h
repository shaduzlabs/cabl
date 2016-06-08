/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

//--------------------------------------------------------------------------------------------------

#include "comm/DeviceDescriptor.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

struct DiscoveryPolicy
{
  bool matches(const DeviceDescriptor&);

  std::string name{"*"};
  DeviceDescriptor::tVendorId vendorId{0};
  DeviceDescriptor::tProductId productId{0};
  DeviceDescriptor::Type type{DeviceDescriptor::Type::Unknown};
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
