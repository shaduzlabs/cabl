/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMikro.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{

const uint16_t kMikro_displayWidth = 128;       // Width of the display in pixels
const uint16_t kMikro_displayHeight = 64;       // Height of the display in pixels
const uint16_t kMikro_nOfDisplayDataChunks = 4; // N. of display data chunks
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplayMaschineMikro::GDisplayMaschineMikro()
  : GDisplay(kMikro_displayWidth, kMikro_displayHeight, kMikro_nOfDisplayDataChunks)
{
  initialize();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::initializeImpl()
{
  data().resize(canvasWidthInBytesImpl() * (1 + ((height() - 1) >> 3)));
  black();
}

//--------------------------------------------------------------------------------------------------

uint16_t GDisplayMaschineMikro::canvasWidthInBytesImpl() const
{
  return width();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::white()
{
  fillPattern(0xFF);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::black()
{
  fillPattern(0x00);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::setPixelImpl(
  uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if (x_ >= width() || y_ >= height() || color_ == Color::None)
    return;

  Color oldColor = pixelImpl(x_, y_);

  if (color_ == Color::Random)
    color_ = static_cast<Color>(util::randomRange(0, 2));

  uint16_t byteIndex = (width() * (y_ >> 3)) + x_;

  switch (color_)
  {
    case Color::White:
      data()[byteIndex] |= 0x01 << (y_ & 7);
      break;

    case Color::Black:
      data()[byteIndex] &= ~(0x01 << (y_ & 7));
      break;

    case Color::Invert:
      data()[byteIndex] ^= 0x01 << (y_ & 7);
      break;

    default:
      break;
  }

  m_isDirty = (m_isDirty ? m_isDirty : oldColor != color_);
  if (bSetDirtyChunk_ && oldColor != color_)
    setDirtyChunks(y_);
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::setPixelImpl(uint16_t x_, uint16_t y_, util::ColorRGB color_, bool bSetDirtyChunk_)
{
  if(color_.mono() > 127U)
  {
    setPixelImpl(x_, y_, Color::White, bSetDirtyChunk_);
  }
  else
  {
    setPixelImpl(x_, y_, Color::Black, bSetDirtyChunk_);
  }
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayMaschineMikro::pixelImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return Color::Black;
  }
  return ((data()[x_ + (width() * (y_ >> 3))] >> ((y_)&7)) & 0x01) == 0 ? Color::Black
                                                                        : Color::White;
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayMaschineMikro::pixelRGBImpl(uint16_t x_, uint16_t y_) const
{
  if (x_ >= width() || y_ >= height())
  {
    return {0,0,0,0};
  }

  if(((data()[x_ + (width() * (y_ >> 3))] >> ((y_)&7)) & 0x01) == 0 )
  {
    return {0,0,0,0};
  }
  
  return  {0xff,0xff,0xff,0xff};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
