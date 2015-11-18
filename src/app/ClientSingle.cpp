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
unsigned kAppSleepBeforeNextDeviceSearch = 5; // seconds
}

namespace sl
{
namespace cabl
{

ClientSingle::ClientSingle(const Driver::tCollDeviceDescriptor& collSupportedDevices_)
  : m_collSupportedDevices(collSupportedDevices_)
{
  M_LOG("Controller Abstraction Library v. " << Lib::getVersion());

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1340); // KK S25
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1350); // KK S49
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1360); // KK S61

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::USB, 0x17CC, 0x0808); // MK1
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1140); // MK2
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1110); // Mikro MK1
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1200); // Mikro MK2

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1120); // Traktor F1
                                                                                    // MK2

  m_collKnownDevices.emplace_back(
    "Ableton Push Live Port", DeviceDescriptor::Type::MIDI, 0x0047, 0x1500); // Ableton Push
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
    if (!isKnownDevice(deviceDescriptor))
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
    if (!isKnownDevice(deviceDescriptor))
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
    if ((!isKnownDevice(deviceDescriptor))
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

  if (isSupportedDevice(deviceDescriptor_) && deviceHandle)
  {
    m_pDevice = DeviceFactory::instance().getDevice(deviceDescriptor_, std::move(deviceHandle));
    m_pDevice->init();
    m_connected = (m_pDevice != nullptr);
  }
  else
  {
    m_pDevice = DeviceFactory::instance().getDevice(deviceDescriptor_, std::move(deviceHandle));
    m_pDevice->init();
    //      m_collDevices.emplace_back(new MaschineMK2(std::move(pDeviceHandle)));
    m_connected = (m_pDevice != nullptr);
    /*
    tPtr<Device> unsupportedDevice(new MaschineMK1(std::move(pDeviceHandle)));
    unsupportedDevice->init();
    unsupportedDevice->tick();

    unsupportedDevice->getGraphicDisplay(0)->black();
    unsupportedDevice->getGraphicDisplay(0)->printStr(
      0,
      0,
      "Unsupported device!",
      namespace cabl::Canvas::tFont::BIG,
      namespace cabl::Canvas::tColor::INVERT
    );
    unsupportedDevice->getGraphicDisplay(1)->printStr(
      12,
      44,
      "Unsupported device!",
      namespace cabl::Canvas::tFont::BIG,
      namespace cabl::Canvas::tColor::INVERT
    );

    unsupportedDevice->tick();
    */
    // Device is known but unsupported by the current Application
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

bool ClientSingle::isKnownDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collKnownDevices)
  {
    if (device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

bool ClientSingle::isSupportedDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collSupportedDevices)
  {
    if (device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
