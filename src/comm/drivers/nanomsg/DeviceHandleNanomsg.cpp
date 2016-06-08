/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/nanomsg/DeviceHandleNanomsg.h"

#include <system_error>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleNanomsg::s_numPacketR;
uint32_t DeviceHandleNanomsg::s_numPacketW;

//--------------------------------------------------------------------------------------------------

DeviceHandleNanomsg::~DeviceHandleNanomsg()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleNanomsg::disconnect()
{
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleNanomsg::read(Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleNanomsg::write(const Transfer& transfer_, uint8_t endpoint_)
{
  return true;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
