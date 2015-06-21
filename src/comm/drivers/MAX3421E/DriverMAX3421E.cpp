/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%

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

#include "DriverMAX3421E.h"
#include "DeviceHandleMAX3421E.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

DriverMAX3421E::DriverMAX3421E()
{

}

//----------------------------------------------------------------------------------------------------------------------

DriverMAX3421E::~DriverMAX3421E()
{

}

//----------------------------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverMAX3421E::enumerate()
{
  return Driver::tCollDeviceDescriptor();
}

//----------------------------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverMAX3421E::connect(const DeviceDescriptor&)
{
  return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
