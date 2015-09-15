/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/generic/USBMidi.h"
#include "comm/Transfer.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

bool USBMidi::sendSysex(const midi::SysEx& sysexMessage_) const
{

  tRawData message(4);
  uint8_t nCable = 0;
  unsigned msgIndex = 0;
  uint8_t rc;
  int bytesToSend = sysexMessage_.data().size();

  while(bytesToSend > 0)
  {
    message[0] = (nCable << 4) | 0x04;  // Start/continue

    switch(bytesToSend)
    {
      case 1: // End with one byte
      {
        message[0] = (nCable << 4) | 0x05;
        message[1] = sysexMessage_.data()[msgIndex++];
        message[2] = 0x00;
        message[3] = 0x00;
        bytesToSend--;
        break;
      }
      case 2: // End with two bytes
      {
        message[0] = (nCable << 4) | 0x06;
        message[1] = sysexMessage_.data()[msgIndex++];
        message[2] = sysexMessage_.data()[msgIndex++];
        message[3] = 0x00;
        bytesToSend -= 2;
        break;
      }
      case 3: // End with three bytes, fall thru
      {
        message[0] = (nCable << 4) | 0x07;
      }
      default:
      {
        message[1] = sysexMessage_.data()[msgIndex++];
        message[2] = sysexMessage_.data()[msgIndex++];
        message[3] = sysexMessage_.data()[msgIndex++];
        bytesToSend -= 3;
        break;
      }
    }
    
    rc = pUsb->outTransfer(bAddress, epInfo[epDataOutIndex].epAddr, 4, buf);
    if(rc != 0)
     break;
  }
//  return(rc);
  
  return true;
}

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
