/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayPush2.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{

  const uint16_t kMASMK2_displayWidth         = 1024;   // Width of the display in pixels
  const uint16_t kMASMK2_displayHeight        = 160;    // Height of the display in pixels
  const uint16_t kMASMK2_nOfDisplayDataChunks = 1;     // N. of display data chunks

}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

GDisplayPush2::GDisplayPush2()
  : GDisplay(
      kMASMK2_displayWidth,
      kMASMK2_displayHeight,
      kMASMK2_nOfDisplayDataChunks,
      Allocation::RGB565
    )
{

}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::white()
{
  fillPattern(0x0);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::black()
{
  fillPattern(0xFF);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayPush2::setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if ( x_ >= width() || y_ >= height() || color_ == Color::None )
    return;

  Color oldColor =pixelImpl( x_, y_ );

  if( color_ == Color::Random )
    color_ = static_cast<Color>( util::randomRange(0,2) );

  uint16_t byteIndex = ( canvasWidthInBytes() * y_ ) + (x_ * 2) ;

  switch( color_ )
  {
    case Color::White:
      data()[ byteIndex ] = 0xff;
      data()[ byteIndex+1 ] = 0xff;
      break;

    case Color::Black:
      data()[ byteIndex ] = 0x00;
      data()[ byteIndex+1 ] = 0x00;
      break;

    case Color::Invert:
      data()[ byteIndex ] = 0xff;
      data()[ byteIndex+1 ] = 0xff;
      break;

    default:
      break;
  }

  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayPush2::pixelImpl(uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= width() || y_ >= height() )
    return Color::Black;

  return
    ( data()[ ( canvasWidthInBytes() * y_ ) + ( x_ >> 3 ) ] & ( 0x80 >> ( x_  & 7 ) ) ) == 0
    ? Color::Black
    : Color::White;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
