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
  uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  util::ColorRGB oldColor = pixel(x_, y_);
  util::ColorRGB newColor = color_;
  if (color_.blendMode() == BlendMode::Invert)
  {
    newColor = oldColor;
    newColor.invert();
  }

  data()[x_ + (y_ * 8)] = devices::MaschineJamHelper::toLedColor(color_);

  if (bSetDirtyChunk_ && oldColor != newColor)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB LedMatrixMaschineJam::pixel(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }
  return devices::MaschineJamHelper::fromLedColor(data()[x_ + (y_ * 8)]);
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
