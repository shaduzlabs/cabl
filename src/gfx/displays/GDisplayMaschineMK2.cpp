/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMK2.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint16_t kMASMK2_displayWidth = 256;       // Width of the display in pixels
const uint16_t kMASMK2_displayHeight = 64;       // Height of the display in pixels
const uint16_t kMASMK2_nOfDisplayDataChunks = 8; // N. of display data chunks
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplayMaschineMK2::GDisplayMaschineMK2()
  : GDisplay(kMASMK2_displayWidth,
      kMASMK2_displayHeight,
      kMASMK2_nOfDisplayDataChunks,
      Allocation::OneBytePacksOneRowOfEightPixels)
{
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::white()
{
  fillPattern(0x0);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::black()
{
  fillPattern(0xFF);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_ == Color::None)
    return;

  Color oldColor = pixelImpl(x_, y_);

  if (color_ == Color::Random)
    color_ = static_cast<Color>(util::randomRange(0, 2));

  uint16_t byteIndex = (canvasWidthInBytes() * y_) + (x_ >> 3);

  switch (color_)
  {
    case Color::White:
      data()[byteIndex] |= (0x80 >> (x_ & 7));
      break;

    case Color::Black:
      data()[byteIndex] &= (~0x80 >> (x_ & 7));
      break;

    case Color::Invert:
      data()[byteIndex] ^= (0x80 >> (x_ & 7));
      break;

    default:
      break;
  }

  m_isDirty = (m_isDirty ? m_isDirty : oldColor != color_);
  if (bSetDirtyChunk_ && oldColor != color_)
    setDirtyChunks(y_);
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayMaschineMK2::pixelImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
    return Color::Black;

  return (data()[(canvasWidthInBytes() * y_) + (x_ >> 3)] & (0x80 >> (x_ & 7))) == 0 ? Color::Black
                                                                                     : Color::White;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
