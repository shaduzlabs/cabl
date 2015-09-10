/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <Usb.h>

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleSAM3XE : public DeviceHandleImpl
{
public:

  DeviceHandleSAM3XE(std::shared_ptr<USBHost> pUsb_, uint32_t deviceAddress_);
  ~DeviceHandleSAM3XE();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) const override;

private:

  static constexpr uint8_t kDHSAM3X8E_maxEndpoints = 255;

  bool isReady() { return m_ready; };

  std::shared_ptr<USBHost>  m_pUsb;
  uint32_t                  m_deviceAddress;

  tRawData                  m_inputBuffer;
  bool                      m_ready;

  uint32_t                  m_confNumber;
  uint32_t                  m_nextPollTime;
  uint32_t                  m_numOfEndpoints;
  EpInfo                    m_epInfo[kDHSAM3X8E_maxEndpoints];

};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
