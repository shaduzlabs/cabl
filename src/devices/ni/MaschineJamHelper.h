/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/util/Color.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

struct MaschineJamHelper
{
  static uint8_t toLedColor(const Color& color_);
  static Color fromLedColor(uint8_t color_);
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
