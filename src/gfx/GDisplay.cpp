/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/GDisplay.h"

#include <cstdio>
#include <cstdlib>
#include <string>

//--------------------------------------------------------------------------------------------------

#define M_SWAP(a, b) \
  {                  \
    a ^= b;          \
    b ^= a;          \
    a ^= b;          \
  }

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplay::GDisplay(uint16_t width_, uint16_t height_, uint8_t numDisplayChunks_)
  : Canvas(width_, height_), m_numDisplayChunks(numDisplayChunks_)
{
  m_pChunksDirtyFlags.resize(numDisplayChunks_);
  resetDirtyFlags();
}

//--------------------------------------------------------------------------------------------------

void GDisplay::initialize()
{
  initializeImpl();
}

//--------------------------------------------------------------------------------------------------

uint16_t GDisplay::canvasWidthInBytes() const
{
  return canvasWidthInBytesImpl();
}

//--------------------------------------------------------------------------------------------------

void GDisplay::setPixel(uint16_t x_, uint16_t y_, const util::ColorRGB& color_)
{
  setPixelImpl(x_, y_, color_, true);
}

//--------------------------------------------------------------------------------------------------

util::ColorRGB GDisplay::pixel(uint16_t x_, uint16_t y_) const
{
  return pixelImpl(x_, y_);
}

//--------------------------------------------------------------------------------------------------

bool GDisplay::dirtyChunk(uint8_t chunk_) const
{
  return ((chunk_ < m_numDisplayChunks) && m_pChunksDirtyFlags[chunk_]);
}

//--------------------------------------------------------------------------------------------------

void GDisplay::resetDirtyFlags() const
{
  for (uint8_t chunk = 0; chunk < m_numDisplayChunks; chunk++)
  {
    m_pChunksDirtyFlags[chunk] = false;
  }
}

//--------------------------------------------------------------------------------------------------

void GDisplay::setDirtyChunk(uint16_t yStart_)
{
  float chunkHeight = static_cast<float>(height()) / m_numDisplayChunks;
  if (yStart_ < height())
  {
    m_pChunksDirtyFlags[static_cast<uint8_t>(yStart_ / chunkHeight)] = true;
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl

//--------------------------------------------------------------------------------------------------

#undef M_SWAP
