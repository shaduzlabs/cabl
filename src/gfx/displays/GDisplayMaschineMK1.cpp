/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/GDisplayMaschineMK1.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{
  
  const uint16_t kMASMK1_displayWidth         = 255;   // Width of the display in pixels
  const uint16_t kMASMK1_displayHeight        = 64;    // Height of the display in pixels
  const uint16_t kMASMK1_nOfDisplayDataChunks = 22;    // N. of display data chunks
  
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
  
//--------------------------------------------------------------------------------------------------

GDisplayMaschineMK1::GDisplayMaschineMK1()
  : GDisplay( 
      kMASMK1_displayWidth, 
      kMASMK1_displayHeight, 
      kMASMK1_nOfDisplayDataChunks, 
      Allocation::TwoBytesPackThreePixelsInARow 
    )
{
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::white()
{
  fillPattern(0xff);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::black()
{
  fillPattern(0x0);
  m_isDirty = true;
}

//--------------------------------------------------------------------------------------------------

void GDisplayMaschineMK1::setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_)
{
  if ( x_ >= width() || y_ >= height() || color_ == Color::None )
    return;
  
  Color oldColor =pixelImpl( x_, y_ );
  
  if (color_ == Color::Random)
  {
    color_ = static_cast<Color>(util::randomRange(0, 2));
  }
  
  uint8_t blockIndex = x_ % 3; // 5 bits per pixel, 2 bytes pack 3 pixels

  uint16_t byteIndex = ( canvasWidthInBytes() * y_ ) + ( ( x_ / 3 ) * 2 );

  switch( color_ )
  {
    case Color::Black:
    {
      switch( blockIndex )
      {
        case 0:
          data()[ byteIndex ] |= 0xF8;
          break;
        case 1:
          data()[ byteIndex ] |= 0x07;
          data()[ byteIndex + 1 ] |= 0xC0;
          break;
        case 2:
          data()[ byteIndex + 1 ] |= 0x1F;
          break;
      }
      break;
    }
    case Color::White:
    {
      switch( blockIndex )
      {
        case 0:
          data()[ byteIndex ] &= 0x07;
          break;
        case 1:
          data()[ byteIndex ] &= 0xF8;
          data()[ byteIndex + 1 ] &= 0x1F;
          break;
        case 2:
          data()[ byteIndex + 1 ] &= 0xC0;
          break;
      }
      break;
    }
    case Color::Invert:
    {
      switch( blockIndex )
      {
        case 0:
        {
          uint8_t pixel2a = data()[ byteIndex ] & 0x07;
          data()[ byteIndex ] = (~(data()[ byteIndex ]) & 0xF8) | pixel2a;
          break;
        }
        case 1:
        {
          uint8_t pixel1 = data()[ byteIndex ] & 0xF8;
          uint8_t pixel3 = data()[ byteIndex + 1] & 0x1F;
          data()[ byteIndex ] = ((~data()[ byteIndex ]) & 0x07) | pixel1;
          data()[ byteIndex + 1 ] = ((~data()[ byteIndex + 1 ]) & 0xC0) | pixel3;
          break;
        }
        case 2:
        {
          uint8_t pixel2b = data()[ byteIndex + 1 ] & 0xC0;
          data()[ byteIndex + 1 ] = (~(data()[ byteIndex + 1 ]) & 0x1F) | pixel2b;
          break;
        }
      }
      break;
    }
    default:
      break;
  }
  
  m_isDirty = ( m_isDirty ? m_isDirty : oldColor != color_ );
  if( bSetDirtyChunk_ && oldColor != color_ )
    setDirtyChunks( y_ );
}

//--------------------------------------------------------------------------------------------------

GDisplay::Color GDisplayMaschineMK1::pixelImpl(uint16_t x_, uint16_t y_ ) const
{
  if ( x_ >= width() || y_ >= height() )
    return Color::Black;
  
  uint8_t blockIndex = x_ % 3; // 5 bits per pixel, 2 bytes pack 3 pixels
  uint16_t byteIndex = ( canvasWidthInBytes() * y_ ) + ( ( x_ / 3 ) * 2 );
  switch( blockIndex )
  {
    case 0:
      return ( data()[ byteIndex ] & 0xF8 ) ? Color::Black : Color::White;
    case 1:
      return ( data()[ byteIndex ] & 0x07 ) ? Color::Black : Color::White;
      break;
    case 2:
      return ( data()[ byteIndex + 1 ] & 0x1F ) ? Color::Black : Color::White;
  }
  
  return Color::Black;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
