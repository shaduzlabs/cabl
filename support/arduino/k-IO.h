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

#include "midi/MidiMessage.hpp"

#include "devices/DeviceFactory.h"

#include "comm/drivers/SAM3X8E/DriverSAM3X8E.h"
#include "comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h"

#include "util/Version.h"

//#include "k-io-config.h"

//--------------------------------------------------------------------------------------------------

#include "comm/Transfer.h"


//#include "devices/DeviceKompleteKontrol.cpp"
//#include "devices/DeviceMaschineMK1.cpp"
#include "devices/DeviceMaschineMK2.h"
#include "devices/DeviceMaschineMikroMK2.h"
//#include "devices/DeviceTraktorF1MK2.cpp"

#include "gfx/Canvas.h"
#include "gfx/GDisplay.h"
#include "gfx/displays/GDisplayMaschineMikro.h"

#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------
/*
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
*/
