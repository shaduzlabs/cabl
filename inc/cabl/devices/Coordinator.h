/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <atomic>
#include <map>
#include <thread>

#include "cabl/comm/DeviceDescriptor.h"
#include "cabl/comm/Driver.h"
#include "cabl/devices/Device.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Coordinator
{
public:
  using tCollDeviceDescriptor = std::vector<DeviceDescriptor>;
  using tDevicePtr = std::shared_ptr<Device>;
  using tCollDevices = std::map<DeviceDescriptor, tDevicePtr>;

  using tClientId = std::string;
  using tDriverPtr = std::shared_ptr<Driver>;
  using tCollDrivers = std::map<Driver::Type, tDriverPtr>;
  using tCbDevicesListChanged = std::function<void(tCollDeviceDescriptor)>;
  using tCollCbDevicesListChanged = std::map<tClientId, tCbDevicesListChanged>;

  static Coordinator& instance()
  {
    static Coordinator instance;
    return instance;
  }

  ~Coordinator();

  tClientId registerClient(tCbDevicesListChanged);
  void unregisterClient(tClientId);

  void run();

  tCollDeviceDescriptor enumerate(bool forceScan_ = false);

  tDevicePtr connect(const DeviceDescriptor&);

private:
  Coordinator();

  void scan();
  bool checkAndAddDeviceDescriptor(const DeviceDescriptor&);
  void devicesListChanged();

  tDriverPtr driver(Driver::Type);

  std::thread m_cablThread;
  std::atomic<bool> m_running{false};
  std::atomic<bool> m_clientRegistered{false};

  std::atomic<bool> m_scanDone{false};
  std::mutex m_mtxDevices;
  std::mutex m_mtxDeviceDescriptors;

  tCollDrivers m_collDrivers;

  tCollCbDevicesListChanged m_collCbDevicesListChanged;
  tCollDeviceDescriptor m_collDeviceDescriptors;
  tCollDevices m_collDevices;

  static std::atomic<unsigned> s_clientCount;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
