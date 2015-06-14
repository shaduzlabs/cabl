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

#include "util/Functions.h"

#if defined (__arm__) && defined (__SAM3X8E__)
  #include <Arduino.h>
#else
#include <cstdlib>
#endif

namespace sl
{
namespace util
{
  
//----------------------------------------------------------------------------------------------------------------------

uint32_t randomRange( uint32_t min_, uint32_t max_ )
{
#if defined (__arm__) && defined (__SAM3X8E__)
  return random( min_, max_ );
#else
  uint32_t base_random = rand(); /* in [0, RAND_MAX] */
  if ( RAND_MAX == base_random )
    return randomRange( min_, max_ );
  /* now guaranteed to be in [0, RAND_MAX) */
  int32_t range       = max_ - min_,
  remainder   = RAND_MAX % range,
  bucket      = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
   within remainder of RAND_MAX */
  if ( base_random < static_cast<uint32_t>(RAND_MAX - remainder) ) {
    return min_ + base_random/bucket;
  } else {
    return randomRange ( min_, max_ );
  }
#endif  
}

uint8_t reverseByte( uint8_t byte )
{
  byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
  byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
  byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
  return byte;
}

//----------------------------------------------------------------------------------------------------------------------

} // util
} // sl

//----------------------------------------------------------------------------------------------------------------------
