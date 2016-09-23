/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>

namespace sl
{
namespace cabl
{

uint32_t randomRange(uint32_t min_, uint32_t max_);

uint8_t reverseByte(uint8_t);

//--------------------------------------------------------------------------------------------------

#undef max
template <typename T>
T max(T x, T y)
{
  return (x > y) ? x : y;
}

//--------------------------------------------------------------------------------------------------

template <typename T>
T max(T x, T y, T z)
{
  T max = (x > y) ? x : y;
  return (max > z) ? max : z;
}

} // namespace cabl
} // namespace sl
