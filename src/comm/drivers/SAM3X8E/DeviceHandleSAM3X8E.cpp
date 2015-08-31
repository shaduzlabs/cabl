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
namespace kio
{

//--------------------------------------------------------------------------------------------------

DeviceHandleSAM3XE::DeviceHandleSAM3XE(tDeviceHandle*)
{
  m_inputBuffer.resize(kSAM3X8EInputBufferSize);

  // Initialize endpoint data structures
  for (uint32_t i = 0; i < kDHSAM3X8E_maxEndpoints; ++i)
  {
    m_epInfo[i].deviceEpNum = 0;
    m_epInfo[i].hostPipeNum = 0;
    m_epInfo[i].maxPktSize  = (i) ? 0 : 8;
    m_epInfo[i].epAttribs   = 0;
    m_epInfo[i].bmNakPower  = (i) ? USB_NAK_NOWAIT : 4;
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
  return true;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleSAM3XE::write(const Transfer& transfer_, uint8_t endpoint_) const
{
  return true;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Init(uint32_t parent, uint32_t port, uint32_t lowspeed)
{
  uint8_t   buf[sizeof(USB_DEVICE_DESCRIPTOR)];
  uint32_t  rcode = 0;
  UsbDevice *p = NULL;
  EpInfo    *oldep_ptr = NULL;
  uint32_t  adkproto = -1;
  uint32_t  num_of_conf = 0;
  uint32_t alloc_status;
  uint8_t pollInterval = 1;
  
  // Get memory address of USB device address pool
  AddressPool &addrPool = m_pUsb->GetAddressPool();
  
  // Check if address has already been assigned to an instance
  if (m_deviceAddress)
  {
    return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
  }
  
  // Get pointer to pseudo device with address 0 assigned
  p = addrPool.GetUsbDevicePtr(0);
  
  if (!p)
  {
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }
  
  if (!p->epinfo)
  {
    return USB_ERROR_EPINFO_IS_NULL;
  }
  
  // Save old pointer to EP_RECORD of address 0
  oldep_ptr = p->epinfo;
  
  // Temporary assign new pointer to epInfo to p->epinfo in order to avoid toggle inconsistence
  p->epinfo = m_epInfo;
  
  p->lowspeed = lowspeed;
  
  // Get device descriptor
  rcode = m_pUsb->getDevDescr(0, 0, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)buf);
  
  // Restore p->epinfo
  p->epinfo = oldep_ptr;
  
  if (rcode)
  {
    goto FailGetDevDescr;
  }
  
  // Allocate new address according to device class
  m_deviceAddress = addrPool.AllocAddress(parent, false, port);
  
  // Extract Max Packet Size from device descriptor
  m_epInfo[0].maxPktSize = (uint8_t)((USB_DEVICE_DESCRIPTOR*)buf)->bMaxPacketSize0;
  
  // Assign new address to the device
  rcode = m_pUsb->setAddr(0, 0, m_deviceAddress);
  if (rcode)
  {
    p->lowspeed = false;
    addrPool.FreeAddress(m_deviceAddress);
    m_deviceAddress = 0;
    printf("DriverSAM3X8E::Init : setAddr failed with rcode %lu\r\n", rcode);
    return rcode;
  }
///!  delay(2);
  printf("DriverSAM3X8E::Init : device address is now %lu\r\n", m_deviceAddress);
  
  p->lowspeed = false;

  //get pointer to assigned address record
  p = addrPool.GetUsbDevicePtr(m_deviceAddress);
  if (!p)
  {
    return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
  }
  
  p->lowspeed = lowspeed;
  
  // Assign epInfo to epinfo pointer - only EP0 is known
  rcode = m_pUsb->setEpInfoEntry(m_deviceAddress, 1, m_epInfo);
  if (rcode)
  {
    goto FailSetDevTblEntry;
  }
  
  // Check if ADK device is already in accessory mode; if yes, configure and exit
  if (((USB_DEVICE_DESCRIPTOR*)buf)->idVendor == static_cast<unsigned>(m_deviceVID) &&
      (((USB_DEVICE_DESCRIPTOR*)buf)->idProduct == static_cast<unsigned>(m_devicePID) ) )
  {
    printf("DriverSAM3X8E::Init : Accessory mode device detected\r\n");
    
    // Go through configurations, find first bulk-IN, bulk-OUT EP, fill epInfo and quit 
    num_of_conf = ((USB_DEVICE_DESCRIPTOR*)buf)->bNumConfigurations;
    
    printf("DriverSAM3X8E::Init : number of configuration is %lu\r\n", num_of_conf);
    
    for (uint32_t i = 0; i < num_of_conf; ++i)
    {
      ConfigDescParser<0, 0, 0, 0> confDescrParser(this);
      
      //  delay(1);
      rcode = m_pUsb->getConfDescr(m_deviceAddress, 0, i, &confDescrParser);
      
      if (rcode)
      {
        goto FailGetConfDescr;
      }
    }
    
    rcode = m_pUsb->setEpInfoEntry(m_deviceAddress, m_numOfEndpoints, m_epInfo);
    if (rcode)
      goto FailSetDevTblEntry;
    
    // Set Configuration Value
    rcode = m_pUsb->setConf(m_deviceAddress, 0, m_confNumber);
    if (rcode)
    {
      goto FailSetConf;
    }
    
    for (uint8_t endpoint = 1 ; endpoint < m_numOfEndpoints ; endpoint++ ) {
      alloc_status = UHD_Pipe_Alloc(
                                    endpoint,
                                    m_epInfo[endpoint].deviceEpNum & 0x7f,
                                    m_epInfo[endpoint].deviceEpNum & 0x80 ? UOTGHS_HSTPIPCFG_PTYPE_INTRPT : UOTGHS_HSTPIPCFG_PTYPE_BLK,
                                    m_epInfo[endpoint].deviceEpNum & 0x80 ? UOTGHS_DEVEPTCFG_EPDIR_IN : UOTGHS_DEVEPTCFG_EPDIR_OUT,
                                    m_epInfo[endpoint].maxPktSize,
                                    pollInterval,
                                    UOTGHS_HSTPIPCFG_PBK_1_BANK
                                    );
      if (alloc_status != endpoint) {
        printf("/!\\ DriverSAM3X8E::EndpointXtract : EP%d [0x%02.2x] allocation error: 0x%04.4x\r\n", endpoint, m_epInfo[endpoint].deviceEpNum, alloc_status);
      }
      else {
        m_epInfo[endpoint].hostPipeNum = endpoint;
        printf("/!\\ DriverSAM3X8E::EndpointXtract : EP%d [0x%02.2x], host pipe: %d allocation OK\r\n", endpoint, m_epInfo[endpoint].deviceEpNum, m_epInfo[endpoint].hostPipeNum );
      }
    }
    printf("DriverSAM3X8E::Init : ADK device configured successfully\r\n");
    m_ready = true;
    return 0;
  }
  
  delay(1);
 
  
  // Diagnostic messages
FailGetDevDescr:
  printf("DriverSAM3X8E::Init getDevDescr : ");
  goto Fail;
  
FailSetDevTblEntry:
  printf("DriverSAM3X8E::Init setDevTblEn : ");
  goto Fail;
  
FailGetProto:
  printf("DriverSAM3X8E::Init getProto : ");
  goto Fail;
  
FailSwAcc:
  printf("DriverSAM3X8E::Init swAcc : ");
  goto Fail;
  
SwAttempt:
  printf("DriverSAM3X8E::Init Accessory mode switch attempt : ");
  goto Fail;
  
FailGetConfDescr:
  printf("DriverSAM3X8E::Init getConf : ");
  goto Fail;
  
FailSetConf:
  printf("DriverSAM3X8E::Init setConf : ");
  goto Fail;
  
Fail:
  printf("error code: %lu\r\n", rcode);
  Release();
  return rcode;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Release()
{
  printf("DriverSAM3X8E::Release\n");
  // Free allocated host pipes
  UHD_Pipe_Free(m_epInfo[1].hostPipeNum);
  UHD_Pipe_Free(m_epInfo[2].hostPipeNum);
  
  // Free allocated USB address
  m_pUsb->GetAddressPool().FreeAddress(m_deviceAddress);
  
  // Must have to be reset to 1
  m_numOfEndpoints = 1;
  
  m_deviceAddress = 0;
  m_ready = false;

  return 0;
}

//--------------------------------------------------------------------------------------------------

uint32_t DeviceHandleSAM3XE::Poll() {
  //  if (!bPollEnable) return 0;
  // FIXME: Freeze on error, no further polling allowed.
  
  if (m_pUsb->getUsbTaskState() != USB_STATE_RUNNING) 
    return 0;
  if (m_nextPollTime > GetTickCount()) 
    return 0;
  
/*
  Transfer transfer_;
  uint32_t nBytesRead = 0;
  unsigned result = m_pUsb->inTransfer(0,129, &nBytesRead, reinterpret_cast<uint8_t*>(m_inputBuffer.data()));  

  if( ( 0 == result ) && ( nBytesRead > 0 ) )
  {
    printf("DriverSAM3X8E::POLL OK on endpoint 129 (%d bytes)\n", nBytesRead);
    transfer_.setData( m_inputBuffer.data(), nBytesRead );
    printf("%d\n",m_inputBuffer.data()[0]);
  }
  else
  {
    printf("DriverSAM3X8E::poll %d bytes on endpoint 129 with code %d\n",nBytesRead,result);
  }
*/
  // Don't poll if shorter than polling interval
 // printf("poll.\n");
  //  HCI_event_task(); // poll the HCI event pipe
  //transfer_.setData( m_inputBuffer.data(), nBytesRead );

 // read();
  //m_nextPollTime = GetTickCount() + pollInterval; // Set new poll time
 
  return 0;
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleSAM3XE::EndpointXtract(
  uint32_t conf,
  uint32_t iface,
  uint32_t alt,
  uint32_t proto,
  const USB_ENDPOINT_DESCRIPTOR *pep
)
{
  printf("DriverSAM3X8E::EndpointXtract");
  
  uint8_t index = 0;
  uint8_t pollInterval = 1;

  //PrintEndpointDescriptor(pep);
  
  printf("> Extract endpoint information conf %u, iface %u, proto %u\r\n",conf,iface,proto);
  m_confNumber = conf;
  
  if ((pep->bmAttributes & 0x03) == 3 && (pep->bEndpointAddress & 0x80) == 0x80) {
    // Interrupt In endpoint found
    index = 1;
    m_epInfo[index].bmNakPower = USB_NAK_NOWAIT;
  //  m_epInfo[index].uotghs_ptype = UOTGHS_HSTPIPCFG_PTYPE_INTRPT;
    printf("> Found event pipe, index #%u\r\n",index);
  } else {
    // Bulk endpoint found
    if ((pep->bmAttributes & 0x02) == 2)
      index = ((pep->bEndpointAddress & 0x80) == 0x80) ? 2 : 3;
    else {
      printf("!!!!!!> Skipping pipe endpoint %04.4x attrs #%04.4x\r\n",
        pep->bEndpointAddress, pep->bEndpointAddress);
      // PrintEndpointDescriptor(pep);
      return;
    }
 //   m_epInfo[index].uotghs_ptype = UOTGHS_HSTPIPCFG_PTYPE_BLK;
    printf("> Found bulk pipe type %u index #%u\r\n", pep->bEndpointAddress, index);
  }
  
  // Fill the rest of endpoint data structure
  m_epInfo[index].maxPktSize  = (uint8_t)pep->wMaxPacketSize;
  // USB spec allows up to 127 devices; retain direction bit in 0x80
  m_epInfo[index].deviceEpNum = pep->bEndpointAddress;
 // m_epInfo[index].bDescriptorType = pep->bDescriptorType;
  
  //PrintEndpointDescriptor(pep);
  
  // Set the polling interval as the largest polling interval obtained from endpoints
  if (pollInterval < pep->bInterval)
    pollInterval = pep->bInterval;
  printf("> Polling interval: Now #%u\r\n", pollInterval);
  m_numOfEndpoints++;
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
