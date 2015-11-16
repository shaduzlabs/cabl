/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "app/ClientSingle.h"

#include "devices/DeviceFactory.h"
#include "devices/ableton/Push2.h"
#include "devices/akai/Push.h"
#include "devices/ni/KompleteKontrol.h"
#include "devices/ni/MaschineMK1.h"
#include "devices/ni/MaschineMK2.h"
#include "devices/ni/MaschineMikroMK2.h"
#include "devices/ni/TraktorF1MK2.h"

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

  static std::string getVersion()
  { 
    return util::Version(CABL_VERSION_MAJOR, CABL_VERSION_MINOR, CABL_VERSION_MICRO); 
  }
  
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // namespace sl