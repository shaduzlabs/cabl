/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineJamHelper.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

uint8_t MaschineJamHelper::toLedColor(const sl::util::ColorRGB& color_)
{
  //\todo implement actual color conversion
  return color_.mono();
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB MaschineJamHelper::fromLedColor(uint8_t color_)
{
  //\todo implement actual color conversion
  return {color_};
}

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
