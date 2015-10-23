/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <map>
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

class ClientSingle
{
public:

  using tDevicePtr = std::shared_ptr<Device>;
  using tDriverPtr = std::shared_ptr<Driver>;
  using tCollDrivers = std::map<Driver::Type, tDriverPtr>;
  
  ClientSingle(const Driver::tCollDeviceDescriptor&);
  virtual ~ClientSingle();

  virtual bool tick() = 0;
  virtual bool initHardware() = 0;

  void run();
  Driver::tCollDeviceDescriptor enumerateDevices();
  bool connect(const DeviceDescriptor&);
  
  void setMaxConsecutiveErrors(unsigned nErrors_){ m_maxConsecutiveErrors = nErrors_;}

protected:
  
  void setConnected(bool connected_)
  {
    m_connected = connected_;
  };
  
  tDevicePtr getDevice(unsigned index)
  {
    return m_pDevice;
  }
  
private:

  tDriverPtr getDriver(Driver::Type);

  bool isKnownDevice(const DeviceDescriptor&) const;

  bool isSupportedDevice(const DeviceDescriptor&) const;

  bool        m_appStopped;
  bool        m_connected;
  std::thread m_cablThread;
  
  unsigned                      m_maxConsecutiveErrors;
  
  Driver::tCollDeviceDescriptor m_collKnownDevices;
  Driver::tCollDeviceDescriptor m_collSupportedDevices;

  tDevicePtr   m_pDevice;
  tCollDrivers m_collDrivers;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
