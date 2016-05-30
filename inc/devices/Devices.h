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

#include "comm/DeviceDescriptor.h"
#include "devices/Device.h"
#include "comm/Driver.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class Devices
{
public:
  using tCollDeviceDescriptor = std::vector<DeviceDescriptor>;
  using tDevicePtr = std::shared_ptr<devices::Device>;
  using tCollDevices = std::map<DeviceDescriptor, tDevicePtr>;

  using tDriverPtr = std::shared_ptr<Driver>;
  using tCollDrivers = std::map<Driver::Type, tDriverPtr>;

  Devices();
  ~Devices();

  void run();

  tCollDeviceDescriptor enumerate(bool forceScan_ = false);

  tDevicePtr connect(const DeviceDescriptor&);

  void onDevicesListChanged();

private:

  void scan();
  bool checkAndAddDeviceDescriptor(const DeviceDescriptor&);
  void devicesListChanged();
  
  tDriverPtr getDriver(Driver::Type);

  std::thread m_cablThread;
  std::atomic<bool> m_running{false};

  std::atomic<bool> m_scanDone{false};
  std::mutex m_mtxDevices;
  std::mutex m_mtxDeviceDescriptors;

  tCollDrivers m_collDrivers;

  tCollDeviceDescriptor m_collDeviceDescriptors;
  tCollDevices m_collDevices;
};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
