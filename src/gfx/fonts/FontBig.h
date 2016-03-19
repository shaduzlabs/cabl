/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#ifndef SL_FONT_BIG_H
#define SL_FONT_BIG_H

#include <gfx/Font.h>

namespace sl
{
namespace cabl
{

class FontBig : public FontBase<FontBig> // CRTP
{

public:
  
  inline uint8_t getWidth() const noexcept        { return 8; };
  
  inline uint8_t getHeight() const noexcept       { return 8; };

  inline uint8_t getCharSpacing() const noexcept  { return getWidth(); };
  
  inline uint8_t getFirstChar() const noexcept    { return 0; };
  
  inline uint8_t getLastChar() const noexcept     { return 128; };
  
  inline uint8_t getBytesPerLine() const noexcept { return 1; };

  inline bool    getPixel( uint8_t c_, uint8_t x_, uint8_t y_ ) const noexcept
  {
    static uint8_t fontData[] =
    {
#include "data/FONT_big.h"
    };
    
    return getPixelImpl( fontData, c_, x_, y_ );
  }

};
  
//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // sl

#endif // SL_FONT_BIG_H
