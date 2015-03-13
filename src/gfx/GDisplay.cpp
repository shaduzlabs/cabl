/*----------------------------------------------------------------------------------------------------------------------   

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gfx/GDisplay.h"

#include "gfx/Font.h"
#include "gfx/fonts/FontSmall.h"
#include "gfx/fonts/FontNormal.h"
#include "gfx/fonts/FontBig.h"

//----------------------------------------------------------------------------------------------------------------------

#define M_SWAP(a, b) { a ^= b; b ^= a; a ^= b; }

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{

//----------------------------------------------------------------------------------------------------------------------

GDisplay::GDisplay(uint16_t width_, uint16_t height_, uint8_t numDisplayChunks_, tAllocation allocationType_ )
: Canvas( width_, height_, allocationType_ )
, m_isDirty(false)
, m_pChunksDirtyFlags( new bool[numDisplayChunks_] )
, m_numDisplayChunks( numDisplayChunks_ )
{
  resetDirtyFlags();
}

//----------------------------------------------------------------------------------------------------------------------


GDisplay::~GDisplay()
{

}

//----------------------------------------------------------------------------------------------------------------------

void GDisplay::setPixel( uint16_t x_, uint16_t y_, tColor color_ )
{
  setPixelImpl( x_, y_, color_, true );
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay::tColor GDisplay::getPixel( uint8_t x_, uint8_t y_ ) const
{
  return getPixelImpl( x_,y_ );
}

//----------------------------------------------------------------------------------------------------------------------

bool GDisplay::isChunkDirty( uint8_t chunk_ ) const
{
  if( chunk_ < m_numDisplayChunks )
    return m_pChunksDirtyFlags[chunk_];
  return false;
}

//----------------------------------------------------------------------------------------------------------------------

void GDisplay::resetDirtyFlags()
{
  m_isDirty = false;
  for( uint8_t chunk = 0; chunk<m_numDisplayChunks; chunk++ )
    m_pChunksDirtyFlags[chunk] = false;
}

//----------------------------------------------------------------------------------------------------------------------

void GDisplay::setDirtyChunks( uint16_t yStart_, uint16_t yEnd_ )
{
  if( yEnd_ == 0xFFFF && yStart_ < getHeight() )
    m_pChunksDirtyFlags[ static_cast<uint8_t>( yStart_ / m_numDisplayChunks) ] = true;
  else if( yEnd_ != 0xFFFF )
  {
    uint8_t startChunk = static_cast<uint8_t>( yStart_ / m_numDisplayChunks);
    uint8_t endChunk   = static_cast<uint8_t>( yEnd_ / m_numDisplayChunks);
    for(uint8_t chunk = startChunk; chunk <= endChunk; chunk++)
      m_pChunksDirtyFlags[chunk] = true;    
  }
  
}

//----------------------------------------------------------------------------------------------------------------------

} // SL
