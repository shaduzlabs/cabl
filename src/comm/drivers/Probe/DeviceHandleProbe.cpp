/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/Probe/DeviceHandleProbe.h"

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
#include <iomanip>
#include <iostream>
#endif

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleProbe::s_numPacketR;
uint32_t DeviceHandleProbe::s_numPacketW;

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

bool DeviceHandleProbe::write(const Transfer& transfer_, uint8_t endpoint_)
{
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  std::cout << "Packet #" << s_numPacketW << " (" << transfer_.size() << " bytes) -> endpoint "
            << static_cast<uint32_t>(endpoint_) << ":" << std::endl;

  std::cout << std::setfill('0') << std::internal;

  for (unsigned i = 0; i < transfer_.size(); i++)
  {
    std::cout << std::hex << std::setw(2) << (int)transfer_[i] << std::dec << " ";
  }

  std::cout << std::endl << std::endl;

  s_numPacketW++;
#endif
  return true;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
