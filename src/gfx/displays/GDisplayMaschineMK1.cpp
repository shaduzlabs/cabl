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

#include "gfx/displays/GDisplayMaschineMK1.h"

#include "util/Functions.h"

//----------------------------------------------------------------------------------------------------------------------

namespace
{
  
  const uint16_t kMASMK1_displayWidth         = 255;   // Width of the display in pixels
  const uint16_t kMASMK1_displayHeight        = 64;    // Height of the display in pixels
  const uint16_t kMASMK1_nOfDisplayDataChunks = 22;    // N. of display data chunks
  
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

GDisplayMaschineMK1::GDisplayMaschineMK1()
  : GDisplay( kMASMK1_displayWidth, kMASMK1_displayHeight, kMASMK1_nOfDisplayDataChunks, tAllocation::ROW_2BYTES_3_PIXELS )
{
}

//----------------------------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::setPixelImpl(uint16_t x_, uint16_t y_, tColor color_, bool bSetDirtyChunk_ )
{
  if ( x_ >= getWidth() || y_ >= getHeight() || color_ == tColor::NONE )
    return;
  
  tColor oldColor = getPixelImpl( x_, y_ );
  
  if (color_ == tColor::RANDOM)
  {
    color_ = static_cast<tColor>(util::randomRange(0, 2));
  }
  
  uint8_t blockIndex = x_ % 3; // 5 bits per pixel, 2 bytes pack 3 pixels

  uint16_t byteIndex = ( getCanvasWidthInBytes() * y_ ) + ( ( x_ / 3 ) * 2 );

  switch( color_ )
  {
    case tColor::WHITE:
      switch( blockIndex )
      {
        case 0:
          getData()[ byteIndex ] |= 0xF8;
          break;
        case 1:
          getData()[ byteIndex ] |= 0x07;
          getData()[ byteIndex + 1 ] |= 0xC0;
          break;
        case 2:
          getData()[ byteIndex + 1 ] |= 0x1F;
          break;
      }
      break;

    case tColor::BLACK:
      switch( blockIndex )
      {
        case 0:
          getData()[ byteIndex ] &= 0x07;
          break;
        case 1:
          getData()[ byteIndex ] &= 0xF8;
          getData()[ byteIndex + 1 ] &= 0x1F;
          break;
        case 2:
          getData()[ byteIndex + 1 ] &= 0xC0;
          break;
      }
      break;

    case tColor::INVERT:
      switch( blockIndex )
      {
        case 0:
          getData()[ byteIndex ] ^= 0xF8;
          break;
        case 1:
          getData()[ byteIndex ] ^= 0x07;
          getData()[ byteIndex + 1 ] ^= 0xC0;
          break;
        case 2:
          getData()[ byteIndex + 1 ] ^= 0x1F;
          break;
      }
      break;

    default:
      break;
  }
  
  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay::tColor GDisplayMaschineMK1::getPixelImpl(uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= getWidth() || y_ >= getHeight() )
    return tColor::BLACK;
  
  uint8_t blockIndex = x_ % 3; // 5 bits per pixel, 2 bytes pack 3 pixels
  uint16_t byteIndex = ( getCanvasWidthInBytes() * y_ ) + ( ( x_ / 3 ) * 2 );
  switch( blockIndex )
  {
    case 0:
      return ( getData()[ byteIndex ] & 0xF8 ) ? tColor::WHITE : tColor::BLACK;
    case 1:
      return ( getData()[ byteIndex ] & 0x07 ) ? tColor::WHITE : tColor::BLACK;
      break;
    case 2:
      return ( getData()[ byteIndex + 1 ] & 0x1F ) ? tColor::WHITE : tColor::BLACK;
  }
  
  return tColor::BLACK;
 }

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
