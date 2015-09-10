/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h"

namespace
{
  uint16_t kSAM3X8EInputBufferSize = 512; // Size of the input buffer
}

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::DeviceHandleSAM3XE(std::shared_ptr<USBHost> pUsb_, uint32_t deviceAddress_)
: m_pUsb(pUsb_)
, m_deviceAddress(deviceAddress_)
{
  m_inputBuffer.resize(kSAM3X8EInputBufferSize);


}

//--------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::~DeviceHandleSAM3XE()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleSAM3XE::disconnect()
{

}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
