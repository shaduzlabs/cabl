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

#ifndef SL_FONT_SMALL_H
#define SL_FONT_SMALL_H

#include <gfx/Font.h>

namespace sl
{
namespace kio
{

class FontSmall : public FontBase<FontSmall> // CRTP
{

public:
  
  inline uint8_t getWidth() const noexcept        { return 3; };
  
  inline uint8_t getHeight() const noexcept       { return 5; };

  inline uint8_t getCharSpacing() const noexcept  { return 1 + getWidth(); };
  
  inline uint8_t getFirstChar() const noexcept    { return 32; };
  
  inline uint8_t getLastChar() const noexcept     { return 126; };
  
  inline uint8_t getBytesPerLine() const noexcept { return 1; };

  inline bool    getPixel( uint8_t c_, uint8_t x_, uint8_t y_ ) const noexcept
  {
    static uint8_t fontData[] =
    {
#include "data/FONT_small.h"
    };
    
    return getPixelImpl( fontData, c_, x_, y_ );
  }

};
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl

#endif // SL_FONT_SMALL_H
