/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <map>

#include "comm/Driver.h"
#include "devices/Device.h"

#include "midi/MidiMessage.hpp"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class Application
{
public:
  using tCollDevices = std::vector<tPtr<Device>>;
  using tCollDrivers = std::map<Driver::Type, tPtr<Driver>>;
  Application(const Driver::tCollDeviceDescriptor&);

  virtual bool tick() = 0;
  virtual bool initHardware() = 0;

  void run();
  Driver::tCollDeviceDescriptor enumerateDevices();
  bool connect(Driver::tCollDeviceDescriptor);
  
  void setMaxConsecutiveErrors(unsigned nErrors_){ m_maxConsecutiveErrors = nErrors_;}

protected:
  void setConnected(bool connected_)
  {
    m_connected = connected_;
  };
  Device* getDevice(unsigned index)
  {
    return m_collDevices[index].get();
  }
  Driver* getDriver(Driver::Type);
  
private:
  bool isKnownDevice(const DeviceDescriptor&) const;
  bool isSupportedDevice(const DeviceDescriptor&) const;

  bool m_appStopped;
  bool m_connected;

  unsigned                      m_maxConsecutiveErrors;
  
  Driver::tCollDeviceDescriptor m_collKnownDevices;
  Driver::tCollDeviceDescriptor m_collSupportedDevices;

  tCollDevices m_collDevices;
  tCollDrivers m_collDrivers;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
