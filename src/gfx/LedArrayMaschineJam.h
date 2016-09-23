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
  void setPixel(unsigned pos_, const Color& color_) override
  {
    if (pos_ < this->length())
    {
      this->data()[pos_] = MaschineJamHelper::toLedColor(color_);
      this->setDirty();
    }
  }

  Color pixel(unsigned pos_) const override
  {
    if (pos_ < this->length())
    {
      return MaschineJamHelper::fromLedColor(this->data()[pos_]);
    }
    return {};
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
