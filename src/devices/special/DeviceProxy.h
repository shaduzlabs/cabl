/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <bitset>
#include <atomic>
#include <thread>

#include <nnxx/socket.h>
#include <cereal/cereal.hpp>

#include "devices/Device.h"
#include "comm/Transfer.h"

namespace sl
{
namespace cabl
{
namespace devices
{
//--------------------------------------------------------------------------------------------------

namespace net
{

//--------------------------------------------------------------------------------------------------

struct Config
{
  std::vector<unsigned> inputEndpoints;
};

//--------------------------------------------------------------------------------------------------

struct Message
{
  unsigned endpoint;
  Transfer data;
};

//--------------------------------------------------------------------------------------------------

} // namespace net

//--------------------------------------------------------------------------------------------------

class DeviceProxy : public Device
{

public:

  DeviceProxy();
  
  bool tick() override;

private:
  void onMessageReceived(const Transfer& message_, unsigned endpoint_);
  void init() override;
  
  std::mutex  m_mtxDevice;
  std::atomic<bool> m_running;
  std::thread m_workerThread;
  
  nnxx::socket  m_socket;
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
