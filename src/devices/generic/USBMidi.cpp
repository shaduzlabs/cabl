/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/generic/USBMidi.h"
#include "comm/Transfer.h"

//--------------------------------------------------------------------------------------------------

using namespace std::placeholders;

namespace sl
{
namespace kio
{
namespace devices
{

// USBMidi specs: http://www.usb.org/developers/docs/devclass_docs/midi10.pdf

//--------------------------------------------------------------------------------------------------

USBMidi::USBMidi(tPtr<DeviceHandle> pDeviceHandle_)
  : Device(std::move(pDeviceHandle_))
{
  setCallbackNoteOff(std::bind(&USBMidi::onNoteOff, this, _1));
  setCallbackNoteOn(std::bind(&USBMidi::onNoteOn, this, _1));
  setCallbackPolyPressure(std::bind(&USBMidi::onPolyPressure, this, _1));
  setCallbackControlChangee(std::bind(&USBMidi::onControlChange, this, _1));
  setCallbackProgramChange(std::bind(&USBMidi::onProgramChange, this, _1));
  setCallbackChannelPressure(std::bind(&USBMidi::onChannelPressure, this, _1));
  setCallbackPitchBend(std::bind(&USBMidi::onPitchBend, this, _1));
}

//--------------------------------------------------------------------------------------------------

void USBMidi::onNoteOff(tPtr<NoteOff> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onNoteOn(tPtr<NoteOn> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onPolyPressure(tPtr<PolyPressure> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onControlChange(tPtr<ControlChange> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onProgramChange(tPtr<ProgramChange> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onChannelPressure(tPtr<ChannelPressure> msg_)
{

}

//--------------------------------------------------------------------------------------------------

void USBMidi::onPitchBend(tPtr<PitchBend> msg_)
{

}

//--------------------------------------------------------------------------------------------------

bool USBMidi::sendSysex(const midi::SysEx& sysexMessage_)
{

  tRawData message(4);
  uint8_t nCable = 0;
  unsigned msgIndex = 0;
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
    
    if(!getDeviceHandle()->write(Transfer(message), 0x02))
    {
      return false;
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
