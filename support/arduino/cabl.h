/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once
#include <map>

#include "comm/Driver.h"
#include "devices/Device.h"
#include "devices/DeviceFactory.h"

#include "comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h"
#include "comm/drivers/SAM3X8E/DriverSAM3X8E.h"

#include "util/Version.h"

#include "cabl-config.h"

//--------------------------------------------------------------------------------------------------

#include "comm/Transfer.h"


//#include "devices/DeviceKompleteKontrol.cpp"
//#include "devices/DeviceMaschineMK1.cpp"
#include "devices/ni/MaschineMK2.h"
#include "devices/ni/MaschineMikroMK2.h"
//#include "devices/DeviceTraktorF1MK2.cpp"

#include "gfx/Canvas.h"
#include "gfx/CanvasBase.h"
#include "gfx/displays/GDisplayMaschineMikro.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------
/*
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
*/
