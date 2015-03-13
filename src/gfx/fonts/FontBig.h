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
#pragma once

#ifndef SL_FONT_BIG_H
#define SL_FONT_BIG_H

#include <gfx/Font.h>

namespace sl
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
  
//----------------------------------------------------------------------------------------------------------------------

} // sl

#endif // SL_FONT_BIG_H
