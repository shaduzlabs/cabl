/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/LedMatrixMaschineJam.h"
#include "devices/ni/MaschineJamHelper.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void LedMatrixMaschineJam::setPixel(
  unsigned x_, unsigned y_, const Color& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  Color oldColor = pixel(x_, y_);
  Color newColor = color_;
  if (color_.blendMode() == BlendMode::Invert)
  {
    newColor = oldColor;
    newColor.invert();
  }

  data()[x_ + (y_ * 8)] = MaschineJamHelper::toLedColor(color_);

  if (bSetDirtyChunk_ && oldColor != newColor)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

Color LedMatrixMaschineJam::pixel(unsigned x_, unsigned y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }
  return MaschineJamHelper::fromLedColor(data()[x_ + (y_ * 8)]);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
