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
#include "util/Types.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class Transfer;

//----------------------------------------------------------------------------------------------------------------------

class DeviceHandle
{

public:

  DeviceHandle(tPtr<DeviceHandleImpl>);

  void disconnect();

  bool read(Transfer&, uint8_t);
  bool write(const Transfer&, uint8_t) const;

  void readAsync(uint8_t, DeviceHandleImpl::tCbRead);

private:
  tPtr<DeviceHandleImpl> m_pImpl;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
