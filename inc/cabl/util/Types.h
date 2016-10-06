/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <memory>
#include <vector>

#ifndef WIN32
#define __stdcall
#endif // WIN32

#if ARDUINO
#include "stl-arduino.h"
#endif // ARDUINO

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

//! The alignment type
enum class Alignment
{
  Left,   //!< Align to the left
  Center, //!< Center
  Right,  //!< Align to the right
};

//--------------------------------------------------------------------------------------------------

//! The blend mode
enum class BlendMode
{
  Normal,      // Normal mode
  Invert,      // Invert the current color
  Transparent, // Ignore the current color
};

//--------------------------------------------------------------------------------------------------

template <typename T>
using tPtr = std::unique_ptr<T>;

using tRawData = std::vector<uint8_t>;

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
