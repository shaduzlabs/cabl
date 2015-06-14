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

#include "gfx/displays/GDisplayMaschineMK2.h"

#include "util/Functions.h"

//----------------------------------------------------------------------------------------------------------------------

namespace
{
  
  const uint16_t kMASMK2_displayWidth         = 256;   // Width of the display in pixels
  const uint16_t kMASMK2_displayHeight        = 64;    // Height of the display in pixels
  const uint16_t kMASMK2_nOfDisplayDataChunks = 8;     // N. of display data chunks
  
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

GDisplayMaschineMK2::GDisplayMaschineMK2()
  : GDisplay( kMASMK2_displayWidth, kMASMK2_displayHeight, kMASMK2_nOfDisplayDataChunks, tAllocation::ROW_1BYTE_8_PIXELS )
{

}

//----------------------------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::setPixelImpl(uint16_t x_, uint16_t y_, tColor color_, bool bSetDirtyChunk_ )
{
  if ( x_ >= getWidth() || y_ >= getHeight() || color_ == tColor::NONE )
    return;
  
  tColor oldColor = getPixelImpl( x_, y_ );

  if( color_ == tColor::RANDOM )
    color_ = static_cast<tColor>( util::randomRange(0,2) );
  
  uint16_t byteIndex = ( getCanvasWidthInBytes() * y_ ) + ( x_ >> 3 );

  switch( color_ )
  {
    case tColor::WHITE:
      getData()[ byteIndex ] |= ( 0x80 >> ( x_ & 7 ) );
      break;

    case tColor::BLACK:
      getData()[ byteIndex ] &= ( ~0x80 >> ( x_ & 7 ) );
      break;

    case tColor::INVERT:
      getData()[ byteIndex ] ^= ( 0x80 >> ( x_ & 7 ) );
      break;

    default:
      break;
  }   
  
  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//----------------------------------------------------------------------------------------------------------------------

GDisplay::tColor GDisplayMaschineMK2::getPixelImpl(uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= getWidth() || y_ >= getHeight() )
    return tColor::BLACK;

  return 
    ( getData()[ ( getCanvasWidthInBytes() * y_ ) + ( x_ >> 3 ) ] & ( 0x80 >> ( x_  & 7 ) ) ) == 0
    ? tColor::BLACK 
    : tColor::WHITE;
}
    
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
