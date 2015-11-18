/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "app/ClientSingle.h"

#include <algorithm>
#include <thread> //remove and use a custom sleep function!
#include <iostream>

#include "cabl.h"

namespace
{
unsigned kAppSleepBeforeNextDeviceSearch = 10; // seconds
}

namespace sl
{
namespace cabl
{

ClientSingle::ClientSingle(const Driver::tCollDeviceDescriptor& collSupportedDevices_)
{
  M_LOG("Controller Abstraction Library v. " << Lib::getVersion());
}

//--------------------------------------------------------------------------------------------------

ClientSingle::~ClientSingle()
{
  if (m_cablThread.joinable())
  {
    m_cablThread.join();
  }
}

//--------------------------------------------------------------------------------------------------

void ClientSingle::run()
{
  m_appStopped = false;
  m_cablThread = std::thread([this]()
    {
      while (!m_appStopped)
      {
        // get the list of devices
        m_connected = false;
        auto collDevices = enumerateDevices();
        if (collDevices.size() > 0 && connect(collDevices[0])) // found known devices
        {
          m_connected = true;
          initHardware();
          unsigned nErrors = 0;
          while (m_connected)
          {
            if (!tick())
            {
              nErrors++;
              if (nErrors >= m_maxConsecutiveErrors)
              {
                //       m_connected = false;
                //       m_collDevices.clear();
                //        M_LOG("[Application] run: disconnected after " << nErrors << " errors" );
              }
            }
            else
            {
              //   nErrors--;
            }
          }
        }
        std::this_thread::sleep_for(std::chrono::seconds(kAppSleepBeforeNextDeviceSearch));
      }

    });
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor ClientSingle::enumerateDevices()
{
  Driver::tCollDeviceDescriptor devicesList;
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  for (const auto& deviceDescriptor : getDriver(Driver::Type::HIDAPI)->enumerate())
  {
    if (!DeviceFactory::instance().isKnownDevice(deviceDescriptor))
    {
      continue; // not a Native Instruments USB device
    }
    devicesList.push_back(deviceDescriptor);
  }
  M_LOG(
    "[Application] enumerateDevices: " << devicesList.size() << " known devices found via HIDAPI");

  unsigned nFoundMidi = 0;
  for (const auto& deviceDescriptor : getDriver(Driver::Type::MIDI)->enumerate())
  {
    if (!DeviceFactory::instance().isKnownDevice(deviceDescriptor))
    {
      continue; // not a Native Instruments USB device
    }
    nFoundMidi++;
    devicesList.push_back(deviceDescriptor);
  }
  M_LOG("[Application] enumerateDevices: " << nFoundMidi << " known devices found via MIDI");

  Driver::Type tMainDriver(Driver::Type::LibUSB);
#endif

  for (const auto& deviceDescriptor : getDriver(tMainDriver)->enumerate())
  {
    if ((!DeviceFactory::instance().isKnownDevice(deviceDescriptor))
        || (std::find(devicesList.begin(), devicesList.end(), deviceDescriptor)
             != devicesList.end()))
    {
      continue; // unknown
    }
    devicesList.push_back(deviceDescriptor);
  }
  M_LOG("[Application] enumerateDevices: " << devicesList.size() << " total known devices found");

  return devicesList;
}

//--------------------------------------------------------------------------------------------------

bool ClientSingle::connect(const DeviceDescriptor& deviceDescriptor_)
{
  m_connected = false;
  if (!deviceDescriptor_)
  {
    return false;
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
    m_pDevice = DeviceFactory::instance().getDevice(deviceDescriptor_, std::move(deviceHandle));
    m_pDevice->init();
    m_connected = (m_pDevice != nullptr);
  }

  return m_connected;
}

//--------------------------------------------------------------------------------------------------

ClientSingle::tDriverPtr ClientSingle::getDriver(Driver::Type tDriver_)
{
  if (m_collDrivers.find(tDriver_) == m_collDrivers.end())
  {
    m_collDrivers.emplace(tDriver_, std::make_shared<Driver>(tDriver_));
  }

  return m_collDrivers[tDriver_];
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
