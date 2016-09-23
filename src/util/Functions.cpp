/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/util/Functions.h"

#if defined(__arm__) && defined(__SAM3X8E__)
#include <Arduino.h>
#else
#include <cstdlib>
#endif

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

uint32_t randomRange(uint32_t min_, uint32_t max_)
{
#if defined(__arm__) && defined(__SAM3X8E__)
  return random(min_, max_);
#else
  uint32_t base_random = rand(); /* in [0, RAND_MAX] */
  if (RAND_MAX == base_random)
    return randomRange(min_, max_);
  /* now guaranteed to be in [0, RAND_MAX) */
  int32_t range = max_ - min_, remainder = RAND_MAX % range, bucket = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
   within remainder of RAND_MAX */
  if (base_random < static_cast<uint32_t>(RAND_MAX - remainder))
  {
    return min_ + base_random / bucket;
  }
  else
  {
    return randomRange(min_, max_);
  }
#endif
}

//--------------------------------------------------------------------------------------------------

uint8_t reverseByte(uint8_t byte)
{
  byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
  byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
  byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
  return byte;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
