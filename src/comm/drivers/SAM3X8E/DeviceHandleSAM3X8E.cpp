/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h"

namespace
{
uint16_t kSAM3X8EInputBufferSize = 512; // Size of the input buffer
}

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::DeviceHandleSAM3XE(std::shared_ptr<USBHost> pUsb_, uint32_t deviceAddress_)
  : m_pUsb(pUsb_), m_deviceAddress(deviceAddress_)
{
  m_inputBuffer.resize(kSAM3X8EInputBufferSize);

  if (m_pUsb)
  {
    m_pUsb->RegisterDeviceClass(this);
  }
}

//--------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::~DeviceHandleSAM3XE()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleSAM3XE::disconnect()
{
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::read(Transfer& transfer_, uint8_t endpoint_)
{
  int nBytesRead = 0;
  int result = m_pUsb->inTransfer(m_deviceAddress, endpoint_, nBytesRead, m_inputBuffer.data());

  if ((0 == result) && (nBytesRead > 0))
  {
    transfer_.setData(m_inputBuffer.data(), nBytesRead);
    return transfer_;
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::write(const Transfer& transfer_, uint8_t endpoint_)
{
  int nBytesWritten = transfer_.size();
  if (transfer_ == true)
  {
    int result = m_pUsb->outTransfer(
      m_deviceAddress, endpoint_, nBytesWritten, const_cast<uint8_t*>(transfer_.getData().data()));

    if ((0 != result) || (nBytesWritten != transfer_.size()))
    {
      M_LOG(
        "[DeviceHandleLibUSB] write: error=" << result << " - transfer size: " << transfer_.size()
                                             << " written: "
                                             << nBytesWritten);
    }
    return ((0 == result) && (nBytesWritten == transfer_.size()));
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Init(uint32_t parent_, uint32_t port_, uint32_t lowspeed_)
{
  uint8_t buf[sizeof(USB_DEVICE_DESCRIPTOR)];

  uint32_t retCode = 0;
  UsbDevice* pDevice = nullptr;
  EpInfo* pEpInfo_old = nullptr;
  uint32_t nOfConfigurations = 0;

  if (m_deviceAddress)
  {
    return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
  }

  AddressPool& addrPool = m_pUsb->GetAddressPool();
  pDevice = addrPool.GetUsbDevicePtr(0);

  if (!pDevice)
  {
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }
  else if (!pDevice->epinfo)
  {
    return USB_ERROR_EPINFO_IS_NULL;
  }

  pEpInfo_old = pDevice->epinfo;
  pDevice->epinfo = m_epInfo;
  pDevice->lowspeed = lowspeed_;
  retCode = m_pUsb->getDevDescr(0, 0, sizeof(USB_DEVICE_DESCRIPTOR), static_cast<uint8_t*>(buf));
  pDevice->epinfo = pEpInfo_old;

  if (retCode != 0)
  {
    M_LOG("ERROR (" << retCode << "): Failed to get device descriptor");
    Release();
    return retCode;
  }

  m_deviceAddress = addrPool.AllocAddress(parent_, false, port_);
  // Extract Max Packet Size from device descriptor
  m_epInfo[0].maxPktSize = (uint8_t)((USB_DEVICE_DESCRIPTOR*)buf)->bMaxPacketSize0;

  // Assign new address to the device
  retCode = m_pUsb->setAddr(0, 0, m_deviceAddress);
  if (retCode)
  {
    pDevice->lowspeed = false;
    addrPool.FreeAddress(m_deviceAddress);
    m_deviceAddress = 0;
    M_LOG("ERROR (" << retCode << "): setAddr failed");
    return retCode;
  }

  M_LOG("Device address is now " << m_deviceAddress);

  pDevice->lowspeed = false;

  // get pointer to assigned address record
  pDevice = addrPool.GetUsbDevicePtr(m_deviceAddress);
  if (!pDevice)
  {
    m_deviceAddress = 0;
    M_LOG("ERROR: address not found in pool");
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }

  pDevice->lowspeed = lowspeed_;

  // Assign epInfo to epinfo pointer - only EP0 is known
  retCode = m_pUsb->setEpInfoEntry(m_deviceAddress, 1, m_epInfo);
  if (retCode)
  {
    m_deviceAddress = 0;
    M_LOG("ERROR (" << retCode << "): setEpInfoEntry failed");
    return retCode;
  }

  // Check if ADK device is already in accessory mode; if yes, configure and exit
  if (((USB_DEVICE_DESCRIPTOR*)buf)->idVendor == ADK_VID
      && (((USB_DEVICE_DESCRIPTOR*)buf)->idProduct == ADK_PID
           || ((USB_DEVICE_DESCRIPTOR*)buf)->idProduct == ADB_PID))
  {
    nOfConfigurations = ((USB_DEVICE_DESCRIPTOR*)buf)->bNumConfigurations;

    M_LOG("Thid device has a total of " << nOfConfigurations << " configurations");

    for (uint32_t i = 0; i < nOfConfigurations; ++i)
    {
      ConfigDescParser<0, 0, 0, 0> confDescrParser(this);

      delay(1);
      retCode = m_pUsb->getConfDescr(m_deviceAddress, 0, i, &confDescrParser);


      if (retCode)
      {
        M_LOG("ERROR (" << retCode << "): Failed to get config descriptor for configuration n. ");
        Release();
        return retCode;
      }

      if (bNumEP > 2)
      {
        break;
      }
    }

    if (bNumEP == 3)
    {
      // Assign epInfo to epinfo pointer - this time all 3 endpoins
      retCode = m_pUsb->setEpInfoEntry(m_deviceAddress, 3, epInfo);
      if (retCode)
      {
        goto FailSetDevTblEntry;
      }
    }

    // Set Configuration Value
    retCode = m_pUsb->setConf(m_deviceAddress, 0, bConfNum);
    if (retCode)
    {
      goto FailSetConf;
    }

    TRACE_USBHOST(printf("ADK::Init : ADK device configured successfully\r\n");)
    ready = true;

    return 0;
  }

  if (retCode)
  {
    goto FailGetProto;
  }
  TRACE_USBHOST(printf("ADK::Init : DK protocol rev. %lu", adkproto);)

  return 0;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Release()
{
  // Free allocated host pipes
  UHD_Pipe_Free(epInfo[epDataInIndex].hostPipeNum);
  UHD_Pipe_Free(epInfo[epDataOutIndex].hostPipeNum);

  // Free allocated USB address
  m_pUsb->GetAddressPool().FreeAddress(m_deviceAddress);

  m_deviceAddress = 0;
  m_ready = false;

  return 0;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Poll()
{
  return 0U;
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleSAM3XE::EndpointXtract(uint32_t conf_,
  uint32_t iface_,
  uint32_t alt_,
  uint32_t proto_,
  const USB_ENDPOINT_DESCRIPTOR* pEp_)
{
  M_LOG("EndpointXtract conf: " << conf << " iface: " << iface << " proto: " << proto_);
  uint8_t index = 0;
  uint8_t pollInterval = 1;

  bConfNum = conf;

  if ((pEp_->bmAttributes & USB_TRANSFER_TYPE_INTERRUPT) == USB_TRANSFER_TYPE_INTERRUPT
      && (pEp_->bEndpointAddress & 0x80) == 0x80)
  {
    // Interrupt In endpoint found
    index = 1;
    epInfo[index].bmNakPower = USB_NAK_NOWAIT;
    M_LOG("Found Interrupt EP, index #%u\r\n", index);
  }
  else
  {
    // Bulk endpoint found
    if ((pep->bmAttributes & 0x02) == 2)
      index = ((pep->bEndpointAddress & 0x80) == 0x80) ? 2 : 3;
    else
    {
      return;
    }
    M_LOG("Found Bulk EP type %u index #%u\r\n", pep->bEndpointAddress, index);
  }

  epInfo[index].maxPktSize
    = (uint8_t)pep->wMaxPacketSize; // Fill the rest of endpoint data structure
  epInfo[index].deviceEpNum
    = pep->bEndpointAddress; // USB spec allows up to 127 devices; retain direction bit in 0x80

  // Set the polling interval as the largest polling interval obtained from endpoints
  if (pollInterval < pep->bInterval)
  {
    pollInterval = pep->bInterval;
  }
  M_LOG("Polling interval: " << pollInterval);
  bNumEP++;
}
//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
