/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "client/Client.h"

#include "devices/DeviceFactory.h"

#include "gfx/DrawingContext.h"
#include "gfx/GDisplay.h"
#include "gfx/LCDDisplay.h"

#include "util/Version.h"

#include "cabl-config.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Lib
{

public:
  static std::string version()
  {
    return util::Version(CABL_VERSION_MAJOR, CABL_VERSION_MINOR, CABL_VERSION_MICRO);
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // namespace sl
