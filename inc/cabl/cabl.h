/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "client/Client.h"

#include "cabl/devices/DeviceFactory.h"

#include "cabl/gfx/Canvas.h"
#include "cabl/gfx/LedArray.h"
#include "cabl/gfx/LedMatrix.h"
#include "cabl/gfx/TextDisplay.h"

#include "cabl/util/Version.h"

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
    return Version(CABL_VERSION_MAJOR, CABL_VERSION_MINOR, CABL_VERSION_MICRO);
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
