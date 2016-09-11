/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMikro.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::setPixel(
  uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  util::ColorRGB oldColor = pixel(x_, y_);

  bool isWhite{color_.active()};
  if (color_.blendMode() == BlendMode::Invert)
  {
    isWhite = !oldColor.active();
  }
  unsigned byteIndex = (width() * (y_ >> 3)) + x_;


  if (isWhite)
  {
    data()[byteIndex] |= 0x01 << (y_ & 7);
  }
  else
  {
    data()[byteIndex] &= ~(0x01 << (y_ & 7));
  }

  if (bSetDirtyChunk_ && oldColor.active() != isWhite)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayMaschineMikro::pixel(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }

  if (((data()[x_ + (width() * (y_ >> 3))] >> ((y_)&7)) & 0x01) == 0)
  {
    return {0};
  }

  return {0xff};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
