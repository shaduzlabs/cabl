/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "app/Application.h"

#include <thread> //remove and use a custom sleep function!
#include <iostream>

#include "k-io.h"

namespace
{
unsigned kAppSleepBeforeNextDeviceSearch = 5; // seconds
}

namespace sl
{
namespace kio
{

Application::Application(const Driver::tCollDeviceDescriptor& collSupportedDevices_)
: m_collSupportedDevices(collSupportedDevices_)
{
  M_LOG( "k-IO Version " << Lib::getVersion());

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1340); //KK S25
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1350); //KK S49
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1360); //KK S61

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::USB, 0x17CC, 0x0808); //MK1
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1140); //MK2
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1110); //Mikro MK1
  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1200); //Mikro MK2

  m_collKnownDevices.emplace_back("", DeviceDescriptor::Type::HID, 0x17CC, 0x1120); //Traktor F1 MK2

  m_collKnownDevices.emplace_back("Ableton Push Live Port", DeviceDescriptor::Type::MIDI, 0x0047, 0x1500); //Ableton Push

}

//--------------------------------------------------------------------------------------------------

Application::~Application()
{
  if (m_kioThread.joinable())
  {
    m_kioThread.join();
  }
}

//--------------------------------------------------------------------------------------------------

void Application::run()
{
  m_appStopped = false;
  m_kioThread = std::thread([this]()
  {
    while(!m_appStopped)
    {
      // get the list of devices
      m_connected = false;
      if (connect(enumerateDevices())) // found known devices
      {
        m_connected = true;
        initHardware();
        unsigned nErrors = 0;
        while (m_connected)
        {
          if(!tick())
          {
            nErrors++;
            if(nErrors>=m_maxConsecutiveErrors)
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

Driver::tCollDeviceDescriptor Application::enumerateDevices()
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
  M_LOG("[Application] enumerateDevices: " << devicesList.size() 
     << " known devices found via HIDAPI");
  
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
    if((!isKnownDevice(deviceDescriptor)) ||
       (std::find(devicesList.begin(), devicesList.end(), deviceDescriptor)!=devicesList.end()))
    {
      continue; // not a Native Instruments USB device
    }
    devicesList.push_back(deviceDescriptor);
  }
  M_LOG("[Application] enumerateDevices: " << devicesList.size() << " total known devices found");

  return devicesList;
}

//--------------------------------------------------------------------------------------------------

bool Application::connect(Driver::tCollDeviceDescriptor devicesList_)
{
  m_connected = false;
  if (devicesList_.empty())
  {
    return false;
  }
  for (const auto& d : devicesList_)
  {
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
    Driver::Type driverType;
    switch(d.getType())
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
    tPtr<DeviceHandle> pDeviceHandle = getDriver(driverType)->connect(d);

    if(isSupportedDevice(d) && pDeviceHandle)
    {
      m_collDevices.emplace_back(DeviceFactory::getDevice(d,std::move(pDeviceHandle)));
      m_connected = true;
    }
    else
    {
      m_collDevices.emplace_back(DeviceFactory::getDevice(d,std::move(pDeviceHandle)));
 
//      m_collDevices.emplace_back(new MaschineMK2(std::move(pDeviceHandle)));
      m_collDevices[0]->init();
      m_connected = true;
      /*
      tPtr<Device> unsupportedDevice(new MaschineMK1(std::move(pDeviceHandle)));
      unsupportedDevice->init();
      unsupportedDevice->tick();

      unsupportedDevice->getGraphicDisplay(0)->black();
      unsupportedDevice->getGraphicDisplay(0)->printStr(
        0,
        0,
        "Unsupported device!",
        kio::Canvas::tFont::BIG,
        kio::Canvas::tColor::INVERT
      );
      unsupportedDevice->getGraphicDisplay(1)->printStr(
        12,
        44,
        "Unsupported device!",
        kio::Canvas::tFont::BIG,
        kio::Canvas::tColor::INVERT
      );

      unsupportedDevice->tick();
      */
      // Device is known but unsupported by the current Application
    }
  }

  return m_connected;
}

//--------------------------------------------------------------------------------------------------

Driver* Application::getDriver(Driver::Type tDriver_)
{
  if (m_collDrivers.find(tDriver_) == m_collDrivers.end())
  {
    m_collDrivers.emplace(tDriver_, std::move(tPtr<Driver>(new Driver(tDriver_))));
  }

  return m_collDrivers[tDriver_].get();
}

//--------------------------------------------------------------------------------------------------

bool Application::isKnownDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collKnownDevices)
  {
    if(device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

bool Application::isSupportedDevice(const DeviceDescriptor& device_) const
{
  for (const auto& d : m_collSupportedDevices)
  {
    if(device_.isSameProduct(d))
    {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
