/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/Devices.h"
#include "devices/DeviceFactory.h"

#include "cabl.h"
#include "comm/drivers/LibUSB/DriverLibUSB.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

Devices::Devices(tCbDevicesListChanged cbDevicesListChanged_)
: m_cbDevicesListChanged(cbDevicesListChanged_)
{
  M_LOG("Controller Abstraction Library v. " << Lib::getVersion());
  auto usbDriver = getDriver(Driver::Type::LibUSB);
  
  usbDriver->setHotplugCallback([this](DeviceDescriptor deviceDescriptor_, bool plugged_)
    {
      scan();/*
      if (plugged_)
      {
        M_LOG("[Devices] hotplug: device plugged");
        if (checkAndAddDeviceDescriptor(deviceDescriptor_))
        {
          M_LOG("[Devices] hotplug: new device found");
        }
        devicesListChanged();
        // notify new device?
      }
      else
      {
        M_LOG("[Devices] usb device unplugged");
        scan();
      }*/
    });
}

//--------------------------------------------------------------------------------------------------

Devices::~Devices()
{
  M_LOG("[Devices] destructor");
  m_running = false;
  if (m_cablThread.joinable())
  {
    m_cablThread.join();
  }
}

//--------------------------------------------------------------------------------------------------

void Devices::run()
{
  bool expected=false;
  if (!m_running.compare_exchange_strong(expected,true))
  {
    return;
  }
  
  scan();
  
  m_cablThread = std::thread([this]()
    {
      while (m_running)
      {
        std::lock_guard<std::mutex> lock(m_mtxDevices);
        for(const auto& device : m_collDevices)
        {
          if(device.second)
          {
            auto result = device.second->tick();
            //! \todo Check tick() result
          }
        }
        std::this_thread::yield();
      }
    });
}

//--------------------------------------------------------------------------------------------------

Devices::tCollDeviceDescriptor Devices::enumerate(bool forceScan_)
{
  if (forceScan_ || !m_scanDone)
  {
    scan();
  }

  return m_collDeviceDescriptors;
}

//--------------------------------------------------------------------------------------------------

Devices::tDevicePtr Devices::connect(const DeviceDescriptor& deviceDescriptor_)
{
  if (!deviceDescriptor_)
  {
    return nullptr;
  }

#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  Driver::Type driverType;
  switch (deviceDescriptor_.getType())
  {
    case DeviceDescriptor::Type::HID:
    {
      driverType = Driver::Type::HIDAPI;
      break;
    }
    case DeviceDescriptor::Type::MIDI:
    {
      driverType = Driver::Type::MIDI;
      break;
    }
    case DeviceDescriptor::Type::USB:
    default:
    {
      driverType = Driver::Type::LibUSB;
      break;
    }
  }
#endif
  auto deviceHandle = getDriver(driverType)->connect(deviceDescriptor_);

  if (deviceHandle)
  {
    auto device = m_collDevices.find(deviceDescriptor_);
    if (device != m_collDevices.end())
    {
      device->second->setDeviceHandle(std::move(deviceHandle));
    }
    else
    {
      auto device = DeviceFactory::instance().getDevice(deviceDescriptor_, std::move(deviceHandle));
      m_collDevices.insert(std::pair<DeviceDescriptor, tDevicePtr>(deviceDescriptor_, device));
    }
    m_collDevices[deviceDescriptor_]->init();
  }

  return m_collDevices[deviceDescriptor_];
}

//--------------------------------------------------------------------------------------------------

void Devices::scan()
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceDescriptors);
  tCollDeviceDescriptor deviceDescriptors{m_collDeviceDescriptors};
  m_collDeviceDescriptors.clear();
  
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  for (const auto& deviceDescriptor : getDriver(Driver::Type::HIDAPI)->enumerate())
  {
    if (checkAndAddDeviceDescriptor(deviceDescriptor))
    {
      M_LOG("[Devices] scan: new device found via HIDAPI");
    }
  }

  for (const auto& deviceDescriptor : getDriver(Driver::Type::MIDI)->enumerate())
  {
    if (checkAndAddDeviceDescriptor(deviceDescriptor))
    {
      M_LOG("[Devices] scan: new device found via MIDI");
    }
  }

  Driver::Type tMainDriver(Driver::Type::LibUSB);
#endif

  for (const auto& deviceDescriptor : getDriver(tMainDriver)->enumerate())
  {
    if (checkAndAddDeviceDescriptor(deviceDescriptor))
    {
      M_LOG("[Devices] scan: new device found via main driver");
    }
  }

  std::sort(m_collDeviceDescriptors.begin(), m_collDeviceDescriptors.end());
  
  {
    std::lock_guard<std::mutex> lock(m_mtxDevices);
    auto it = m_collDevices.begin();
    while (it != m_collDevices.end())
    {
      bool found{false};
      for(const auto& deviceDescriptor : m_collDeviceDescriptors)
      {
        if(deviceDescriptor == it->first)
        {
          found = true;
          break;
        }
      }

      if (!found)
      {
        it->second->resetDeviceHandle();
      }

      it++;
    }
  }

  m_scanDone = true;

  if(m_collDeviceDescriptors.size() != deviceDescriptors.size() || !std::equal(m_collDeviceDescriptors.begin(),m_collDeviceDescriptors.end(), deviceDescriptors.begin()))
  {
    devicesListChanged();
  }

}

//--------------------------------------------------------------------------------------------------

bool Devices::checkAndAddDeviceDescriptor(const sl::cabl::DeviceDescriptor& deviceDescriptor)
{
    if ((!DeviceFactory::instance().isKnownDevice(deviceDescriptor))
        || std::find(m_collDeviceDescriptors.begin(), m_collDeviceDescriptors.end(), deviceDescriptor) != m_collDeviceDescriptors.end())
    {
      return false; // unknown
    }
    m_collDeviceDescriptors.push_back(deviceDescriptor);
    return true;
}

//--------------------------------------------------------------------------------------------------

void Devices::devicesListChanged()
{
  M_LOG("[Devices]: The devices list has changed");
  auto devices = enumerate();
  for(const auto& device : devices)
  {
    M_LOG(device.getName());
  }
  if(m_cbDevicesListChanged)
  {
    m_cbDevicesListChanged(devices);
  }
}

//--------------------------------------------------------------------------------------------------

Devices::tDriverPtr Devices::getDriver(Driver::Type tDriver_)
{
  if (m_collDrivers.find(tDriver_) == m_collDrivers.end())
  {
    m_collDrivers.emplace(tDriver_, std::make_shared<Driver>(tDriver_));
  }

  return m_collDrivers[tDriver_];
}

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
