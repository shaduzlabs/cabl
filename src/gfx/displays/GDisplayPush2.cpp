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

void GDisplayPush2::setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_ == Color::None)
  {
    return;
  }
  
  Color oldColor = pixelImpl(x_, y_);

  if (color_ == Color::Random)
  {
    color_ = static_cast<Color>(util::randomRange(0, 2));
  }
  
  unsigned byteIndex = (canvasWidthInBytes() * y_) + (x_ * 2);

  switch (color_)
  {
    case Color::White:
      data()[byteIndex] = 0xff;
      data()[byteIndex + 1] = 0xff;
      break;

    case Color::Black:
      data()[byteIndex] = 0x00;
      data()[byteIndex + 1] = 0x00;
      break;

    case Color::Invert:
      data()[byteIndex] = oldColor == Color::Black ? 0xff : 0x00;
      data()[byteIndex + 1] = oldColor == Color::Black ? 0xff : 0x00;
      break;

    default:
      break;
  }

  m_isDirty = (m_isDirty ? m_isDirty : oldColor != color_);
  if (bSetDirtyChunk_ && oldColor != color_)
  {
    setDirtyChunks(y_);
  }
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::setPixelImpl(uint16_t x_, uint16_t y_, util::ColorRGB color_, bool bSetDirtyChunk_)
{
  unsigned byteIndex = (canvasWidthInBytes() * y_) + (x_ * 2);
  uint8_t green = ( (color_.green() / 255.0) * 63 );
  data()[byteIndex] = (static_cast<uint8_t>( (color_.red() / 255.0) * 31 ) << 3 ) | ( ( green >> 3 ) & 0x03 );
  data()[byteIndex + 1] =  ( ( green << 5 ) & 0xE0 ) | static_cast<uint8_t>( (color_.blue() / 255.0) * 31 );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayPush2::pixelImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return Color::Black;
  }
  return (data()[(canvasWidthInBytes() * y_) + (x_ * 2)] == 0 ) ? Color::Black : Color::White;
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayPush2::pixelRGBImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {0,0,0,0};
  }
  unsigned index = (canvasWidthInBytes() * y_) + (x_ * 2);
  
  return {
    static_cast<uint8_t>( ( ( data()[index]>>3 ) / 31.0 ) * 255),
    static_cast<uint8_t>( ( ( ( data()[index] & 0x07 ) << 3 | ( data()[index + 1] & 0xE0 ) >> 5 ) / 63.0 ) * 255),
    static_cast<uint8_t>( ( ( data()[index + 1] & 0x1F ) / 31.0 ) * 255),
    0xff
  };
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
