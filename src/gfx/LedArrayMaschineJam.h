/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "devices/ni/MaschineJamHelper.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

template <unsigned L>
class LedArrayMaschineJam : public LedArrayBase<L>
{

public:
  void setPixel(unsigned pos_, const util::ColorRGB& color_) override
  {
    if (pos_ < this->length())
    {
      this->data()[pos_] = devices::MaschineJamHelper::toLedColor(color_);
      this->setDirty();
    }
  }

  util::ColorRGB pixel(unsigned pos_) const override
  {
    if (pos_ < this->length())
    {
      return devices::MaschineJamHelper::fromLedColor(this->data()[pos_]);
    }
    return {};
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
