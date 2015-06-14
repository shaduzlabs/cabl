/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

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

//----------------------------------------------------------------------------------------------------------------------

class Lib
{

public:

  static std::string getVersion() { return util::Version(KIO_VERSION_MAJOR, KIO_VERSION_MINOR, KIO_VERSION_MICRO); }
  
};

//--------------------------------------------------------------------------------------------------------------------

} // namespace kio
} // namespace sl