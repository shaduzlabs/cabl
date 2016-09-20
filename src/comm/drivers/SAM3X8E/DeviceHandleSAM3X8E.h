/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

#include <Usb.h>
#include <confdescparser.h>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleSAM3XE : public DeviceHandleImpl, public USBDeviceConfig, public UsbConfigXtracter
{
public:
  DeviceHandleSAM3XE(std::shared_ptr<USBHost> pUsb_, uint32_t deviceAddress_);
  ~DeviceHandleSAM3XE();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

private:
  static constexpr uint8_t kDHSAM3X8E_maxEndpoints = 5;

  uint32_t Init(uint32_t parent, uint32_t port, uint32_t lowspeed) override;

  uint32_t Release() override;

  uint32_t Poll() override
  {
    return 0;
  }

  uint32_t GetAddress() override
  {
    return m_deviceAddress;
  };

  bool isReady()
  {
    return m_ready;
  };

  void EndpointXtract(uint32_t conf,
    uint32_t iface,
    uint32_t alt,
    uint32_t proto,
    const USB_ENDPOINT_DESCRIPTOR* ep) override;

  std::shared_ptr<USBHost> m_pUsb;
  uint32_t m_deviceAddress;

  tRawData m_inputBuffer;
  bool m_ready;

  uint32_t m_confNumber;
  uint32_t m_nextPollTime;
  uint32_t m_numOfEndpoints;
  EpInfo m_epInfo[kDHSAM3X8E_maxEndpoints];
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
