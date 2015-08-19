/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "app/Application.h"

#include "devices/DeviceFactory.h"
#include "devices/DeviceMaschineMK1.h"
#include "devices/DeviceMaschineMK2.h"
#include "devices/DeviceMaschineMikroMK2.h"

#include "util/Version.h"

#include "k-io-config.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class Lib
{

public:

  static std::string getVersion()
  { 
    return util::Version(KIO_VERSION_MAJOR, KIO_VERSION_MINOR, KIO_VERSION_MICRO); 
  }
  
};

//--------------------------------------------------------------------------------------------------

} // namespace kio
} // namespace sl