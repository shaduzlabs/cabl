/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/DiscoveryPolicy.h"

#include <regex>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

bool DiscoveryPolicy::matches(const DeviceDescriptor& deviceDescriptor_)
{
  if (type != DeviceDescriptor::Type::Unknown && type != deviceDescriptor_.type())
  {
    return false;
  }
  if (vendorId != 0 && vendorId != deviceDescriptor_.vendorId())
  {
    return false;
  }
  if (productId != 0 && productId != deviceDescriptor_.productId())
  {
    return false;
  }

  if (name != "*")
  {
    std::regex expression(name);
    std::smatch match;
    std::regex_match(deviceDescriptor_.name(), match, expression);
    if (match.size() < 1)
    {
      return false;
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
