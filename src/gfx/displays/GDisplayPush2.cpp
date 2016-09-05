/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayPush2.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{
const uint16_t kMASMK2_displayWidth = 1024;      // Width of the display in pixels
const uint16_t kMASMK2_displayHeight = 160;      // Height of the display in pixels
const uint16_t kMASMK2_nOfDisplayDataChunks = 1; // N. of display data chunks
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplayPush2::GDisplayPush2()
  : GDisplay(kMASMK2_displayWidth, kMASMK2_displayHeight, kMASMK2_nOfDisplayDataChunks)
{
  initialize();
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::initializeImpl()
{
  data().resize(canvasWidthInBytesImpl() * height());
  black();
}

//--------------------------------------------------------------------------------------------------

uint16_t GDisplayPush2::canvasWidthInBytesImpl() const
{
  uint16_t canvasWitdhInBytes = width() * 2;
  return canvasWitdhInBytes;
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::white()
{
  fillPattern(0xff);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::black()
{
  fillPattern(0x00);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::setPixelImpl(
  uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_.transparent())
  {
    return;
  }

  util::ColorRGB oldColor = pixelImpl(x_, y_);
  util::ColorRGB newColor = color_;
  if (color_.blendMode() == util::ColorRGB::BlendMode::Invert)
  {
    newColor = oldColor;
    newColor.invert();
  }

  unsigned byteIndex = (canvasWidthInBytes() * y_) + (x_ * 2);
  uint8_t green = ((newColor.green() / 255.0) * 63) + 0.5;
  data()[byteIndex]
    = (static_cast<uint8_t>(((newColor.red() / 255.0) * 31) + 0.5) << 3) | ((green >> 3) & 0x07);
  data()[byteIndex + 1]
    = ((green << 5) & 0xE0) | static_cast<uint8_t>(((newColor.blue() / 255.0) * 31) + 0.5);

  m_isDirty = (m_isDirty ? m_isDirty : oldColor != newColor);
  if (bSetDirtyChunk_ && oldColor != newColor)
  {
    setDirtyChunk(y_);
  }
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayPush2::pixelImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {};
  }
  unsigned index = (canvasWidthInBytes() * y_) + (x_ * 2);

  return {static_cast<uint8_t>((((data()[index] >> 3) / 31.0) * 255) + 0.5),
    static_cast<uint8_t>(
            ((((data()[index] & 0x07) << 3 | (data()[index + 1] & 0xE0) >> 5) / 63.0) * 255) + 0.5),
    static_cast<uint8_t>((((data()[index + 1] & 0x1F) / 31.0) * 255) + 0.5)};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
