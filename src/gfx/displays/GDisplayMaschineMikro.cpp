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
  
  const uint16_t kMikro_displayWidth          = 128;   // Width of the display in pixels
  const uint16_t kMikro_displayHeight         = 64;    // Height of the display in pixels
  const uint16_t kMikro_nOfDisplayDataChunks  = 4;     // N. of display data chunks

}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
  
//--------------------------------------------------------------------------------------------------

GDisplayMaschineMikro::GDisplayMaschineMikro()
  : GDisplay(
      kMikro_displayWidth,
      kMikro_displayHeight,
      kMikro_nOfDisplayDataChunks,
      Allocation::OneBytePacksOneColOfEightPixels
    )
{

}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::white()
{
  fillPattern(0x0);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::black()
{
  fillPattern(0xFF);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMikro::setPixelImpl(
  uint16_t x_, 
  uint16_t y_, 
  Color color_, 
  bool bSetDirtyChunk_
)
{
  if ( x_ >= getWidth() || y_ >= getHeight() || color_ == Color::None )
    return;
  
  Color oldColor = getPixelImpl( x_, y_ );
  
  if( color_ == Color::Random )
    color_ = static_cast<Color>( util::randomRange(0,2) );
  
  uint16_t byteIndex = ( getWidth() * ( y_ >> 3 ) ) + x_;
  
  switch( color_ )
  {
    case Color::White:
      getData()[ byteIndex ] |= 0x01 << ( y_ & 7 );
      break;
      
    case Color::Black:
      getData()[ byteIndex ] &= ~(0x01 << ( y_ & 7 ) );
      break;
      
    case Color::Invert:
      getData()[ byteIndex ] ^= 0x01 << ( y_ & 7 );
      break;
      
    default:
      break;
  }
  
  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayMaschineMikro::getPixelImpl( uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= getWidth() || y_ >= getHeight() )
    return Color::Black;
  
  return
  ( ( getData()[ x_ + ( getWidth() * ( y_ >> 3 ) ) ] >> ( ( y_ ) & 7 ) ) & 0x01 ) == 0
  ? Color::Black
  : Color::White;
}
 
//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
