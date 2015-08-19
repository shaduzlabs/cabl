/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

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

//--------------------------------------------------------------------------------------------------

DeviceHandleMAX3421E::DeviceHandleMAX3421E(tDeviceHandle*)
{
  m_inputBuffer.resize(kMAX3421EnputBufferSize);
}

//--------------------------------------------------------------------------------------------------

DeviceHandleMAX3421E::~DeviceHandleMAX3421E()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMAX3421E::disconnect()
{
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMAX3421E::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMAX3421E::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
