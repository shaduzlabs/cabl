/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <map>
#include <atomic>
#include <thread>

#include "comm/Driver.h"
#include "devices/Device.h"

#include <unMIDIfy.hpp>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------
using namespace devices;

class Client final
{
public:

  using tDevicePtr = std::shared_ptr<Device>;
  using tDriverPtr = std::shared_ptr<Driver>;
  using tCollDrivers = std::map<Driver::Type, tDriverPtr>;

  using tCbVoid = std::function<void(void)>;

  Client();
  virtual ~Client();

  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;

  void run();
  void stop();

  static Driver::tCollDeviceDescriptor enumerateDevices();
  bool connect(const DeviceDescriptor&);

  void setCallbacks(tCbVoid cbConnected_, tCbVoid cbTick_, tCbVoid cbDisconnected_)
  {
    m_cbConnected = cbConnected_;
    m_cbTick = cbTick_;
    m_cbDisconnected = cbDisconnected_;
  }

  tDevicePtr getDevice()
  {
    return m_pDevice;
  }

  void setLed(Device::Button, const util::LedColor&);

  void setLed(Device::Pad, const util::LedColor&);
  void setLed(Device::Key, const util::LedColor&);

private:

  void onTick();
  void onConnected();
  void onDisconnected();

  static tDriverPtr getDriver(Driver::Type);

  tCbVoid             m_cbConnected;
  tCbVoid             m_cbTick;
  tCbVoid             m_cbDisconnected;

  std::atomic<bool>   m_clientStopped;
  std::atomic<bool>   m_connected;
  std::thread         m_cablThread;

  tDevicePtr          m_pDevice;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
