/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceHandleProbe.h"

#include <iostream>
#include <iomanip>

namespace
{
  uint16_t kProbeInputBufferSize = 512; // Size of the TEST input buffer
}

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleProbe::s_numPacketR;
uint32_t DeviceHandleProbe::s_numPacketW;

//--------------------------------------------------------------------------------------------------

DeviceHandleProbe::DeviceHandleProbe(tDeviceHandle*)
{
  m_inputBuffer.resize(kProbeInputBufferSize);
}

//--------------------------------------------------------------------------------------------------

DeviceHandleProbe::~DeviceHandleProbe()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleProbe::disconnect()
{
  
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleProbe::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleProbe::write(const Transfer& transfer_, uint8_t endpoint_) const
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

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
