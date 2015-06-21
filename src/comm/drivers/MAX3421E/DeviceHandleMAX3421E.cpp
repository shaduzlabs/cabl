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

#include "DeviceHandleMAX3421E.h"
#include <iostream>
#include <iomanip>

namespace
{
  uint16_t kMAX3421EnputBufferSize = 512; // Size of the input buffer
}

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleMAX3421E::DeviceHandleMAX3421E(tDeviceHandle*)
{
  m_inputBuffer.resize(kMAX3421EnputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleMAX3421E::~DeviceHandleMAX3421E()
{
  disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceHandleMAX3421E::disconnect()
{
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleMAX3421E::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleMAX3421E::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
