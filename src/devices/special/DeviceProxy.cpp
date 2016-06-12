/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/special/DeviceProxy.h"

#include <nnxx/pair.h>
#include <nnxx/message.h>

#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions.h"

//--------------------------------------------------------------------------------------------------

namespace
{

const char* k_socketAddress = "tcp://*:12481";

}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

DeviceProxy::DeviceProxy()
{
  m_socket.bind(k_socketAddress);
}

//--------------------------------------------------------------------------------------------------

bool DeviceProxy::tick()
{
  std::lock_guard<std::mutex> lock(m_mtxDevice);
  Transfer input;
  unsigned endpoint{0};
  if(readFromDeviceHandle( input, endpoint ))
  {
    // Send to network
  }
  
  return true;
}

//--------------------------------------------------------------------------------------------------

void DeviceProxy::init()
{
  M_LOG("[DeviceProxy] init");
  bool expected = false;
  if(m_running.compare_exchange_weak(expected, true) && hasDeviceHandle())
  {
    m_workerThread = std::thread([this]()
    {
      while (m_running)
      {
        nnxx::message msg = m_socket.recv();
        {
          // call onMessageReceived
        }
      }
    });
 }
}

//--------------------------------------------------------------------------------------------------

void DeviceProxy::onMessageReceived(const Transfer& message_, unsigned endpoint_)
{
}


} // devices
} // cabl
} // sl
