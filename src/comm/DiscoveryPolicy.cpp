/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/comm/DiscoveryPolicy.h"

#include <regex>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

bool DiscoveryPolicy::matches(const DeviceDescriptor& deviceDescriptor_)
{
  if (m_type != DeviceDescriptor::Type::Unknown && m_type != deviceDescriptor_.type())
  {
    return false;
  }
  if (m_vendorId != 0 && m_vendorId != deviceDescriptor_.vendorId())
  {
    return false;
  }
  if (m_productId != 0 && m_productId != deviceDescriptor_.productId())
  {
    return false;
  }

  if (m_name != "*")
  {
    std::regex expression(m_name);
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

} // namespace cabl
} // namespace sl
