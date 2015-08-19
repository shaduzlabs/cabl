/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gfx/GDisplay.h"

#include "gfx/Font.h"
#include "gfx/fonts/FontSmall.h"
#include "gfx/fonts/FontNormal.h"
#include "gfx/fonts/FontBig.h"

//--------------------------------------------------------------------------------------------------

#define M_SWAP(a, b) { a ^= b; b ^= a; a ^= b; }

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

GDisplay::GDisplay(
  uint16_t width_, 
  uint16_t height_, 
  uint8_t numDisplayChunks_, 
  Allocation allocationType_
)
: Canvas( width_, height_, allocationType_ )
, m_isDirty(false)
, m_numDisplayChunks( numDisplayChunks_ )
{
  m_pChunksDirtyFlags.resize(numDisplayChunks_);
  resetDirtyFlags();
}

//--------------------------------------------------------------------------------------------------


GDisplay::~GDisplay()
{

}

//--------------------------------------------------------------------------------------------------

void GDisplay::setPixel( uint16_t x_, uint16_t y_, Color color_ )
{
  setPixelImpl( x_, y_, color_, true );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplay::getPixel(uint16_t x_, uint16_t y_ ) const
{
  return getPixelImpl( x_,y_ );
}

//--------------------------------------------------------------------------------------------------

bool GDisplay::isChunkDirty( uint8_t chunk_ ) const
{
  if( chunk_ < m_numDisplayChunks )
    return m_pChunksDirtyFlags[chunk_];
  return false;
}

//--------------------------------------------------------------------------------------------------

void GDisplay::resetDirtyFlags()
{
  m_isDirty = false;
  for( uint8_t chunk = 0; chunk<m_numDisplayChunks; chunk++ )
    m_pChunksDirtyFlags[chunk] = false;
}

//--------------------------------------------------------------------------------------------------

void GDisplay::setDirtyChunks( uint16_t yStart_, uint16_t yEnd_ )
{
  float chunkHeight = static_cast<float>(getHeight()) / m_numDisplayChunks;
  if( yEnd_ == 0xFFFF && yStart_ < getHeight() )
    m_pChunksDirtyFlags[ static_cast<uint8_t>( yStart_ / chunkHeight) ] = true;
  else if( yEnd_ != 0xFFFF )
  {
    uint8_t startChunk = static_cast<uint8_t>( yStart_ / chunkHeight);
    uint8_t endChunk   = static_cast<uint8_t>( yEnd_ / chunkHeight);
	for (uint8_t chunk = startChunk; chunk <= endChunk; chunk++)
	{
      m_pChunksDirtyFlags[chunk] = true;    
	}
  }
  
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
