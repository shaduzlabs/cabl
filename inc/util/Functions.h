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

#ifndef SL_UTIL_H
#define SL_UTIL_H

#include <stdint.h>

namespace sl
{
namespace util
{

  uint32_t randomRange ( uint32_t min_, uint32_t max_ );

  uint8_t reverseByte( uint8_t );
  
//----------------------------------------------------------------------------------------------------------------------

  template<typename T>
  T max( T x, T y )
  {
    return ( x > y ) ? x : y;
  }

//----------------------------------------------------------------------------------------------------------------------

  template<typename T>
  T max( T x, T y, T z )
  {
    T max = ( x > y ) ? x : y;
    return ( max > z ) ? max : z;
  }

} // UTIL
} // SL

#endif