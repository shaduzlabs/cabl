/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <Usb.h>
#include <confdescparser.h>

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleSAM3XE : public DeviceHandleImpl, public UsbConfigXtracter, public USBDeviceConfig
{
public:

  using tDeviceHandle = void;

  DeviceHandleSAM3XE(tDeviceHandle*);
  ~DeviceHandleSAM3XE();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) const override;

  void setUsbHost(USBHost* pUsb_){ m_pUsb = pUsb_; }
  
private:
  
  static constexpr uint8_t kDHSAM3X8E_maxEndpoints = 255;
  
  uint32_t Init(uint32_t parent, uint32_t port, uint32_t lowspeed) override;
  
  uint32_t Release() override;
  
  uint32_t Poll() override;
  
  uint32_t GetAddress() override { return m_deviceAddress; };
  
  void EndpointXtract(
    uint32_t conf,
    uint32_t iface,
    uint32_t alt,
    uint32_t proto,
    const USB_ENDPOINT_DESCRIPTOR *ep
  ) override;
  
  bool isReady() { return m_ready; };
  
  tRawData                m_inputBuffer;
  bool                    m_ready;
  
  uint32_t                m_deviceAddress;
  uint32_t                m_confNumber;
  
  uint32_t                m_nextPollTime;
  
  uint32_t                m_numOfEndpoints;
  
  DeviceDescriptor::tVendorId       m_deviceVID;
  DeviceDescriptor::tProductId      m_devicePID;

  USBHost*                m_pUsb;
  EpInfo                  m_epInfo[kDHSAM3X8E_maxEndpoints];
  
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
