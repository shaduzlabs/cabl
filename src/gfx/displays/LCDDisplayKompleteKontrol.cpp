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

#include <stdint.h>

#include "util/Log.h"
#include "gfx/displays/LCDDisplayKompleteKontrol.h"

namespace
{
  static const uint16_t kLCDDisplayKK_FontData[] =
  {
#include "../fonts/data/FONT_16-seg.h"
  };
}
    
namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------
 
LCDDisplayKompleteKontrol::LCDDisplayKompleteKontrol()
  : LCDDisplay(8,3)
{

}
  
//----------------------------------------------------------------------------------------------------------------------

LCDDisplayKompleteKontrol::~LCDDisplayKompleteKontrol()
{
  
}

//----------------------------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::printChar(uint16_t x_, uint16_t y_, char c_)
{

}

//----------------------------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(const std::string& string_, uint8_t row_)
{
  M_LOG("LCD: " << string_.c_str());
}

//----------------------------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(unsigned value_, uint8_t row_)
{
  
}

//----------------------------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setValue(float value_, uint8_t row_)
{
  
}

//----------------------------------------------------------------------------------------------------------------------
  
} // kio
} // sl
