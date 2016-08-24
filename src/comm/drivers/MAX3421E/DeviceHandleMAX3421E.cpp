/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/MAX3421E/DeviceHandleMAX3421E.h"

namespace
{
uint16_t kMAX3421EnputBufferSize = 512; // Size of the input buffer
}

namespace sl
{
namespace cabl
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

bool DeviceHandleMAX3421E::write(const Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
