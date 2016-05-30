/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverLibUSB.h"

#include <thread>

#include "DeviceHandleLibUSB.h"

namespace
{

//--------------------------------------------------------------------------------------------------

std::string getStringDescriptor(libusb_device_handle* pHandle_, uint8_t uDescriptor)
{
  if (uDescriptor != 0)
  {
    char sDescriptor[256];

    if (libusb_get_string_descriptor_ascii(
          pHandle_, uDescriptor, (unsigned char*)sDescriptor, sizeof(sDescriptor))
        > 0)
    {
      return sDescriptor;
    }
  }
  return std::string();
}

//--------------------------------------------------------------------------------------------------

int cbHotplug(
  libusb_context* /*ctx_*/, libusb_device* device_, libusb_hotplug_event event_, void* userData_)
{
  static libusb_device_handle* pHandle = nullptr;
  struct libusb_device_descriptor descriptor;

  libusb_get_device_descriptor(device_, &descriptor);
  sl::cabl::DriverLibUSB* pDriver = static_cast<sl::cabl::DriverLibUSB*>(userData_);

  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event_)
  {
    if (libusb_open(device_, &pHandle) < 0)
    {
      return -1;
    }
    std::string strSerialNum = ::getStringDescriptor(pHandle, descriptor.iSerialNumber);
    std::string strProd = ::getStringDescriptor(pHandle, descriptor.iProduct);

    libusb_close(pHandle);
    sl::cabl::DeviceDescriptor deviceDescriptor(strProd,
      sl::cabl::DeviceDescriptor::Type::USB,
      descriptor.idVendor,
      descriptor.idProduct,
      strSerialNum);

    if (pDriver)
    {
      pDriver->hotplug(deviceDescriptor, true);
    }
  }
  else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event_)
  {
    // Device name and serial number are unknown because getStringDescriptor cannot be used (pHandle is null)
    sl::cabl::DeviceDescriptor deviceDescriptor(
      "", sl::cabl::DeviceDescriptor::Type::USB, descriptor.idVendor, descriptor.idProduct, "");

    if (pDriver)
    {
      pDriver->hotplug(deviceDescriptor, false);
    }
  }
  else
  {
    // unhandled event
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DriverLibUSB::DriverLibUSB() : m_usbThreadRunning(true)
{
  libusb_init(&m_pContext);
#if !defined(NDEBUG)
  libusb_set_debug(m_pContext, 3);
#endif

  libusb_hotplug_register_callback(m_pContext,
    static_cast<libusb_hotplug_event>(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
                                      | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
    static_cast<libusb_hotplug_flag>(0),
    LIBUSB_HOTPLUG_MATCH_ANY,
    LIBUSB_HOTPLUG_MATCH_ANY,
    LIBUSB_HOTPLUG_MATCH_ANY,
    ::cbHotplug,
    this,
    m_pHotplugHandle);

  m_usbThread = std::thread([this]()
    {
      while (m_usbThreadRunning)
      {
        libusb_handle_events(m_pContext);
        //      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    });

  M_LOG("[LibUSB] initialization");
}

//--------------------------------------------------------------------------------------------------

DriverLibUSB::~DriverLibUSB()
{
  M_LOG("[LibUSB] shutting down...");
  m_usbThreadRunning = false;
  struct timeval tv = {1, 0};
  int completed = 0;

  libusb_handle_events_timeout_completed(m_pContext, &tv, &completed);
  // libusb_hotplug_deregister_callback(&callback);
  libusb_exit(m_pContext);
  if (m_usbThread.joinable())
  {
    m_usbThread.join();
  }
  M_LOG("[LibUSB] exit");
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverLibUSB::enumerate()
{
  M_LOG("[LibUSB] enumerate");
  Driver::tCollDeviceDescriptor collDeviceDescriptor;

  libusb_device** devices;
  ssize_t nDevices = libusb_get_device_list(m_pContext, &devices);

  libusb_device_handle* pHandle = nullptr;
  for (int i = 0; i < nDevices; ++i)
  {
    libusb_device* device = devices[i];
    if (libusb_open(device, &pHandle) < 0)
    {
      continue;
    }
    struct libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    std::string strSerialNum = ::getStringDescriptor(pHandle, descriptor.iSerialNumber);

    std::string strProd = ::getStringDescriptor(pHandle, descriptor.iProduct);
#ifndef NDEBUG
    std::string strManuf = ::getStringDescriptor(pHandle, descriptor.iManufacturer);

    if (strSerialNum.empty())
    {
      M_LOG("[LibUSB] enumerate: " << strProd << "(" << strManuf << ")");
    }
    else
    {
      M_LOG("[LibUSB] enumerate: " << strProd << "(" << strManuf << ") with S/N \"" << strSerialNum
                                   << "\"");
    }
#endif
    libusb_close(pHandle);
    DeviceDescriptor deviceDescriptor(
      strProd, DeviceDescriptor::Type::USB, descriptor.idVendor, descriptor.idProduct, strSerialNum);
    collDeviceDescriptor.push_back(deviceDescriptor);
  }

  libusb_free_device_list(devices, 1);

  return collDeviceDescriptor;
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverLibUSB::connect(const DeviceDescriptor& device_)
{
  M_LOG("[LibUSB] connecting to " << device_.getVendorId() << ":" << device_.getProductId() << ":"
                                  << device_.getSerialNumber());
  bool bConnected = false;
  libusb_device** devices;
  ssize_t nDevices = libusb_get_device_list(m_pContext, &devices);

  libusb_device_handle* pCurrentDevice = nullptr;
  for (int i = 0; i < nDevices; ++i)
  {
    libusb_device* device = devices[i];
    struct libusb_device_descriptor descriptor;
    libusb_get_device_descriptor(device, &descriptor);
    if (descriptor.idVendor != device_.getVendorId()
        || descriptor.idProduct != device_.getProductId())
    {
      continue;
    }

    int e = libusb_open(device, &pCurrentDevice);

    if (e == 0)
    {
      std::string strSerialNum = ::getStringDescriptor(pCurrentDevice, descriptor.iSerialNumber);
      if (strSerialNum == device_.getSerialNumber())
      {
        bConnected = true;
        break;
      }
      else
      {
        libusb_close(pCurrentDevice);
      }
    }
  }

  libusb_free_device_list(devices, 1);

  libusb_set_configuration(pCurrentDevice, 1);
  libusb_claim_interface(pCurrentDevice, 0);

  libusb_set_interface_alt_setting(pCurrentDevice, 0, 1);

  if (pCurrentDevice == nullptr || !bConnected)
    return nullptr;

  M_LOG("[LibUSB] CONNECTED to " << device_.getVendorId() << ":" << device_.getProductId() << ":"
                                 << device_.getSerialNumber());

  return tPtr<DeviceHandleImpl>(new DeviceHandleLibUSB(pCurrentDevice));
}

//--------------------------------------------------------------------------------------------------

void DriverLibUSB::setHotplugCallback(Driver::tCbHotplug cbHotplug_)
{
  m_cbHotplug = cbHotplug_;
}

//--------------------------------------------------------------------------------------------------

void DriverLibUSB::hotplug(const DeviceDescriptor& deviceDescriptor_, bool plugged_)
{
  if (m_cbHotplug)
  {
    m_cbHotplug(deviceDescriptor_, plugged_);
  }
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
