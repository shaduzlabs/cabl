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

#include "DriverSAM3X8E.h"
#include <iostream>
#include <iomanip>

namespace
{

uint16_t kSAM3X8EInputBufferSize = 512; // Size of the input buffer
}

namespace sl
{

//----------------------------------------------------------------------------------------------------------------------

uint32_t DriverSAM3X8E::s_numPacketR;
uint32_t DriverSAM3X8E::s_numPacketW;

//----------------------------------------------------------------------------------------------------------------------

DriverSAM3X8E::DriverSAM3X8E()
{
  m_inputBuffer.resize(kSAM3X8EInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DriverSAM3X8E::~DriverSAM3X8E()
{
  disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverSAM3X8E::connect(Driver::tVendorId vid_, Driver::tProductId pid_)
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DriverSAM3X8E::disconnect()
{
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverSAM3X8E::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverSAM3X8E::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

} // sl
