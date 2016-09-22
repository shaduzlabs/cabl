/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMK2.h"

#include "cabl/util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::setPixel(
  unsigned x_, unsigned y_, const Color& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  Color oldColor = pixel(x_, y_);

  bool isWhite{color_.active()};
  if (color_.blendMode() == BlendMode::Invert)
  {
    isWhite = !oldColor.active();
  }
  unsigned byteIndex = (canvasWidthInBytes() * y_) + (x_ >> 3);

  if (isWhite)
  {
    data()[byteIndex] |= (0x80 >> (x_ & 7));
  }
  else
  {
    data()[byteIndex] &= (~0x80 >> (x_ & 7));
  }

  if (bSetDirtyChunk_ && oldColor.active() != isWhite)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

Color GDisplayMaschineMK2::pixel(unsigned x_, unsigned y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }

  if ((data()[(canvasWidthInBytes() * y_) + (x_ >> 3)] & (0x80 >> (x_ & 7))) == 0)
  {
    return {0};
  }

  return {0xff};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
