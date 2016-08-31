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
  uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
{
  if ( x_ >= width() || y_ >= height() || color_.transparent() )
  {
    return;
  }
  
  util::ColorRGB oldColor = pixelImpl(x_, y_);

  bool isWhite{color_.active()};
  if(color_.blendMode() == util::ColorRGB::BlendMode::Invert)
  {
    isWhite = !oldColor.active();
  }
  unsigned byteIndex = (width() * (y_ >> 3)) + x_;
  

  if(isWhite)
  {
    data()[byteIndex] |= 0x01 << (y_ & 7);
  }
  else
  {
    data()[byteIndex] &= ~(0x01 << (y_ & 7));
  }

  m_isDirty = (m_isDirty ? m_isDirty : oldColor.active() != isWhite);
  if (bSetDirtyChunk_ && oldColor.active() != isWhite)
  {
    setDirtyChunks(y_);
  }
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayMaschineMikro::pixelImpl(uint16_t x_, uint16_t y_) const
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
