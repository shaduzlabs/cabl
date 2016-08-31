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
  : GDisplay(kMASMK2_displayWidth, kMASMK2_displayHeight, kMASMK2_nOfDisplayDataChunks)
{
  initialize();
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::initializeImpl()
{
  data().resize(canvasWidthInBytesImpl() * height());
  black();
}

//--------------------------------------------------------------------------------------------------

uint16_t GDisplayMaschineMK2::canvasWidthInBytesImpl() const
{
  uint16_t canvasWitdhInBytes = 1 + ((width() - 1) >> 3);
  return canvasWitdhInBytes;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::white()
{
  fillPattern(0xff);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::black()
{
  fillPattern(0x00);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::setPixelImpl(uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
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
  uint16_t byteIndex = (canvasWidthInBytes() * y_) + (x_ >> 3);
  
  if(isWhite)
  {
    data()[byteIndex] |= (0x80 >> (x_ & 7));
  }
  else
  {
    data()[byteIndex] &= (~0x80 >> (x_ & 7));
  }

  m_isDirty = (m_isDirty ? m_isDirty : oldColor.active() != isWhite);
  if (bSetDirtyChunk_ && oldColor.active() != isWhite)
  {
    setDirtyChunks(y_);
  }
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplayMaschineMK2::pixelImpl(uint16_t x_, uint16_t y_) const
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
