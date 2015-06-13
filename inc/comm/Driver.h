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

#include <cstdint>
#include "Types.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class DeviceHandle;
class DriverImpl;
class Transfer;

//----------------------------------------------------------------------------------------------------------------------

class Driver
{

public:
  
  enum class tDriver : uint8_t
  {
    MOCK,
    HIDAPI,
    LIBUSB,
    SAM3X8E,
    MAX3421E,
  };
  
  using tVendorId = uint16_t;
  using tProductId = uint16_t;
  
   Driver( tDriver type_ );
  ~Driver();

  tPtr<DeviceHandle> connect( tVendorId vid_, tProductId pid_ );
  
private:
 
  tPtr<DriverImpl>  m_pImpl;

};
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
