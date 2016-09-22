/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMK1.h"

#include "cabl/util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplayMaschineMK1::GDisplayMaschineMK1()
{
  black();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::white()
{
  fill(0x00);
  setDirty();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::black()
{
  fill(0xff);
  setDirty();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::setPixel(
  unsigned x_, unsigned y_, const Color& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  Color oldColor = pixel(x_, y_);
  unsigned byteIndex = (canvasWidthInBytes() * y_) + ((x_ / 3) * 2);
  uint8_t pixelValue{0};
  if (color_.blendMode() == BlendMode::Invert)
  {
    Color newColor = oldColor;
    newColor.invert();
    pixelValue = (static_cast<uint8_t>((newColor.mono() / 255.0) * 31 + 0.5f));
  }
  else
  {
    pixelValue = (static_cast<uint8_t>((color_.mono() / 255.0) * 31 + 0.5f));
  }

  switch (x_ % 3)
  {
    case 0:
      data()[byteIndex] |= 0xF8;
      data()[byteIndex] &= ~(pixelValue << 3);
      break;
    case 1:
      data()[byteIndex] |= 0x07;
      data()[byteIndex + 1] |= 0xC0;
      data()[byteIndex] &= ~(pixelValue >> 2);
      data()[byteIndex + 1] &= ~(pixelValue << 6);
      break;
    case 2:
      data()[byteIndex + 1] |= 0x1F;
      data()[byteIndex + 1] &= ~(pixelValue);
      break;
  }

  if (bSetDirtyChunk_ && oldColor != color_)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

Color GDisplayMaschineMK1::pixel(unsigned x_, unsigned y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }

  uint8_t blockIndex = x_ % 3; // 5 bits per pixel, 2 bytes pack 3 pixels
  unsigned byteIndex = (canvasWidthInBytes() * y_) + ((x_ / 3) * 2);
  uint8_t pixelValue{0};
  switch (blockIndex)
  {
    case 0:
      pixelValue = ~(static_cast<uint8_t>((((data()[byteIndex] & 0xF8) >> 3) / 31.0) * 255));
      break;
    case 1:
      pixelValue = ~(static_cast<uint8_t>(
        ((((data()[byteIndex] & 0x07) << 2) | (data()[byteIndex + 1] & 0xC0) >> 6) / 31.0) * 255));
      break;
    case 2:
      pixelValue = ~(static_cast<uint8_t>(((data()[byteIndex + 1] & 0x1F) / 31.0) * 255));
      break;
  }

  return {pixelValue};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
