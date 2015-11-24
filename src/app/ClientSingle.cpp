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
static const unsigned kClientMaxConsecutiveErrors = 100;
}

namespace sl
{
namespace cabl
{

ClientSingle::ClientSingle()
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
  m_clientStopped = false;
  m_cablThread = std::thread(
    [this]()
    {
      while (!m_clientStopped)
      {
      
        //\todo remove enumerateDevices call!
/*
        m_connected = false;
        auto collDevices = enumerateDevices();
        if (collDevices.size() > 0) // found known devices
        {
          connect(collDevices[0]);
        }
  */      
        
        if(m_connected)
        {
          onConnected();
          unsigned nErrors = 0;
          while (m_connected)
          {
            onTick();
            if(!m_pDevice->tick())
            {
              nErrors++;
              if (nErrors >= kClientMaxConsecutiveErrors)
              {
                m_connected = false;
                M_LOG("[Application] run: disconnected after " << nErrors << " errors" );
              }
            }
            else
            {
              nErrors = 0;
            }
          }
          onDisconnected();
        }
        else
        {
          M_LOG("[Application] waiting for a device" );
          std::this_thread::yield();
        }
      }
    }
  );
}

//--------------------------------------------------------------------------------------------------

void ClientSingle::stop()
{
  m_connected = false;
  m_clientStopped = true;
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

void ClientSingle::onConnected()
{
  if(m_cbConnected)
  {
    m_cbConnected();
  }
}

//--------------------------------------------------------------------------------------------------

void ClientSingle::onTick()
{
  if(m_cbTick)
  {
    m_cbTick();
  }
}

//--------------------------------------------------------------------------------------------------

void ClientSingle::onDisconnected()
{
  if(m_cbDisconnected)
  {
    m_cbDisconnected();
  }
}

//--------------------------------------------------------------------------------------------------

ClientSingle::tDriverPtr ClientSingle::getDriver(Driver::Type tDriver_)
{
  static tCollDrivers collDrivers;

  if (collDrivers.find(tDriver_) == collDrivers.end())
  {
    collDrivers.emplace(tDriver_, std::make_shared<Driver>(tDriver_));
  }

  return collDrivers[tDriver_];
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
