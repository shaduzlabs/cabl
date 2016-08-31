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
  : Canvas(width_, height_), m_isDirty(false), m_numDisplayChunks(numDisplayChunks_)
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

bool GDisplay::isChunkDirty(uint8_t chunk_) const
{
  if (chunk_ < m_numDisplayChunks)
  {
    return m_pChunksDirtyFlags[chunk_];
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

void GDisplay::resetDirtyFlags() const
{
  m_isDirty = false;
  for (uint8_t chunk = 0; chunk < m_numDisplayChunks; chunk++)
  {
    m_pChunksDirtyFlags[chunk] = false;
  }
}

//--------------------------------------------------------------------------------------------------

void GDisplay::setDirtyChunks(uint16_t yStart_, uint16_t yEnd_)
{
  float chunkHeight = static_cast<float>(height()) / m_numDisplayChunks;
  if (yEnd_ == 0xFFFF && yStart_ < height())
  {
    m_pChunksDirtyFlags[static_cast<uint8_t>(yStart_ / chunkHeight)] = true;
  }
  else if (yEnd_ != 0xFFFF)
  {
    uint8_t startChunk = static_cast<uint8_t>(yStart_ / chunkHeight);
    uint8_t endChunk = static_cast<uint8_t>(yEnd_ / chunkHeight);
    for (uint8_t chunk = startChunk; chunk <= endChunk; chunk++)
    {
      m_pChunksDirtyFlags[chunk] = true;
    }
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl

//--------------------------------------------------------------------------------------------------

#undef M_SWAP
