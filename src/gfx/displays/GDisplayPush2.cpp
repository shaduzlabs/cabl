/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayPush2.h"

#include "cabl/util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::setPixel(
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

  unsigned byteIndex = (canvasWidthInBytes() * y_) + (x_ * 2);
  uint8_t green = static_cast<uint8_t>(((newColor.green() / 255.0) * 63) + 0.5);
  
  data()[byteIndex + 1]
    = (static_cast<uint8_t>(((newColor.blue() / 255.0) * 31) + 0.5) << 3) | ((green >> 3) & 0x07);
    
  data()[byteIndex ]
    = ((green << 5) & 0xE0) | static_cast<uint8_t>(((newColor.red() / 255.0) * 31) + 0.5);

  if (bSetDirtyChunk_ && oldColor != newColor)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

Color GDisplayPush2::pixel(unsigned x_, unsigned y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }
  unsigned index = (canvasWidthInBytes() * y_) + (x_ * 2);

  return {static_cast<uint8_t>((((data()[index ] & 0x1F) / 31.0) * 255) + 0.5),
    static_cast<uint8_t>(
            ((((data()[index] & 0x07) << 3 | (data()[index + 1] & 0xE0) >> 5) / 63.0) * 255) + 0.5),
    static_cast<uint8_t>((((data()[index + 1] >> 3) / 31.0) * 255) + 0.5)};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
