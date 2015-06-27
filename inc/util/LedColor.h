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

#include "Functions.h"

namespace sl
{
namespace util
{

class LedColor
{
public:

  LedColor(uint8_t mono_)
    : m_red(mono_)
    , m_green(mono_)
    , m_blue(mono_)
    , m_mono(mono_)
  {}
  
  LedColor(uint8_t red_, uint8_t green_, uint8_t blue_)
    : m_red(red_)
    , m_green(green_)
    , m_blue(blue_)
    , m_mono(util::max<uint8_t>( red_, green_, blue_ )) // Maximum decomposition -> take the channel w/ highest value
  {}
  
  LedColor(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_)
    : m_red( red_ )
    , m_green( green_ )
    , m_blue( blue_ )
    , m_mono( mono_ )
  {}
 
  unsigned getRed  () const { return m_red;   }
  unsigned getGreen() const { return m_green; }
  unsigned getBlue () const { return m_blue;  }
  unsigned getMono () const { return m_mono;  }

  void setRed   (uint8_t red_  ){ m_red = red_;     }
  void setGreen (uint8_t green_){ m_green = green_; }
  void setBlue  (uint8_t blue_ ){ m_blue = blue_;   }
  void setMono  (uint8_t mono_ ){ m_mono = mono_;   }

  void black(){ m_red = m_blue = m_green = m_mono = 0;    }
  void white(){ m_red = m_blue = m_green = m_mono = 0xFF; }

private:

  unsigned m_red;
  unsigned m_green;
  unsigned m_blue;
  unsigned m_mono;
};

} // namespace util
} // namespace sl
