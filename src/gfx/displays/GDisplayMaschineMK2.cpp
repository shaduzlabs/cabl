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
  
  const uint16_t kMASMK2_displayWidth         = 256;   // Width of the display in pixels
  const uint16_t kMASMK2_displayHeight        = 64;    // Height of the display in pixels
  const uint16_t kMASMK2_nOfDisplayDataChunks = 8;     // N. of display data chunks
  
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
  
//--------------------------------------------------------------------------------------------------

GDisplayMaschineMK2::GDisplayMaschineMK2()
  : GDisplay(
      kMASMK2_displayWidth, 
      kMASMK2_displayHeight, 
      kMASMK2_nOfDisplayDataChunks, 
      Allocation::OneBytePacksOneRowOfEightPixels
    )
{

}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK2::setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if ( x_ >= getWidth() || y_ >= getHeight() || color_ == Color::None )
    return;
  
  Color oldColor = getPixelImpl( x_, y_ );

  if( color_ == Color::Random )
    color_ = static_cast<Color>( util::randomRange(0,2) );
  
  uint16_t byteIndex = ( getCanvasWidthInBytes() * y_ ) + ( x_ >> 3 );

  switch( color_ )
  {
    case Color::White:
      getData()[ byteIndex ] |= ( 0x80 >> ( x_ & 7 ) );
      break;

    case Color::Black:
      getData()[ byteIndex ] &= ( ~0x80 >> ( x_ & 7 ) );
      break;

    case Color::Invert:
      getData()[ byteIndex ] ^= ( 0x80 >> ( x_ & 7 ) );
      break;

    default:
      break;
  }   
  
  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayMaschineMK2::getPixelImpl(uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= getWidth() || y_ >= getHeight() )
    return Color::Black;

  return 
    ( getData()[ ( getCanvasWidthInBytes() * y_ ) + ( x_ >> 3 ) ] & ( 0x80 >> ( x_  & 7 ) ) ) == 0
    ? Color::Black
    : Color::White;
}
    
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
