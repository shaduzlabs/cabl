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

#include "DriverMOCK.h"
#include <iostream>
#include <iomanip>

namespace
{

uint16_t kTESTInputBufferSize = 512; // Size of the TEST input buffer
}

namespace sl
{

//----------------------------------------------------------------------------------------------------------------------

uint32_t DriverMOCK::s_numPacketR;
uint32_t DriverMOCK::s_numPacketW;

//----------------------------------------------------------------------------------------------------------------------

DriverMOCK::DriverMOCK()
{
  m_inputBuffer.resize(kTESTInputBufferSize);
}

//----------------------------------------------------------------------------------------------------------------------

DriverMOCK::~DriverMOCK()
{
  disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverMOCK::connect(Driver::tVendorId vid_, Driver::tProductId pid_)
{
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

void DriverMOCK::disconnect()
{
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverMOCK::read(Transfer& transfer_, uint8_t endpoint_)
{


  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool DriverMOCK::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  std::cout << "Packet #" << s_numPacketW << " (" << transfer_.size() << " bytes) -> endpoint "
            << static_cast<uint32_t>(endpoint_) << ":" << std::endl;

  std::cout << std::setfill('0') << std::internal;

  for (unsigned i = 0; i < transfer_.size(); i++)
  {
    std::cout << std::hex << std::setw(2) << (int)transfer_[i] << std::dec << " ";
  }

  std::cout << std::endl << std::endl;

  s_numPacketW++;

  return true;
}

//----------------------------------------------------------------------------------------------------------------------

} // sl
