/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <memory>
#include <vector>

#include "util/CircularBuffer.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

  template<typename T>
  using tPtr       = std::unique_ptr<T>;
  
  using tRawData   = std::vector<uint8_t>;
  using tCollFlags = std::vector<bool>;
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
