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

#include "DeviceHandleSAM3X8E.h"

namespace
{
  uint16_t kSAM3X8EInputBufferSize = 512; // Size of the input buffer
}

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::DeviceHandleSAM3XE(tDeviceHandle*)
{
  m_inputBuffer.resize(kSAM3X8EInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::~DeviceHandleSAM3XE()
{
  disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

void DeviceHandleSAM3XE::disconnect()
{

}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
