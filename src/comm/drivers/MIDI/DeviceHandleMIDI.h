/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>

#include "comm/DriverImpl.h"
#include "comm/DeviceHandleImpl.h"

#include <RtMidi.h>


namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class DeviceHandleMIDI : public DeviceHandleImpl
{
public:

  DeviceHandleMIDI(const DeviceDescriptor&);
  ~DeviceHandleMIDI();

  void disconnect() override;

  bool read(Transfer&, uint8_t) override;
  bool write(const Transfer&, uint8_t) override;

  void readAsync(uint8_t endpoint_, DeviceHandle::tCbRead) override;

  static void onMidiMessage(
    double timeStamp_,
    std::vector<unsigned char> *pMessage_,
    void *pUserData_
  );

private:
  
  RtMidiIn                        m_midiIn;
  RtMidiOut                       m_midiOut;
 
  DeviceHandle::tCbRead           m_cbRead;
};

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
