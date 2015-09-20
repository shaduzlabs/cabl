/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceHandleMIDI.h"

namespace
{
  uint16_t kMIDIInputBufferSize = 512;  // Size of the LIBUSB input buffer
}

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

DeviceHandleMIDI::DeviceHandleMIDI(RtMidiIn midiIn_, RtMidiOut midiOut_)
  : m_midiIn(std::move(midiIn_))
  , m_midiOut(std::move(midiOut_))
{
  m_inputBuffer.resize(kMIDIInputBufferSize);
}

//--------------------------------------------------------------------------------------------------

DeviceHandleMIDI::~DeviceHandleMIDI()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::disconnect()
{
  m_midiIn.closePort();
  m_midiOut.closePort();
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMIDI::read( Transfer& transfer_, uint8_t /* endpoint_ */)
{
  m_midiIn.getMessage(const_cast<std::vector<unsigned char>*>(&(transfer_.getData())));
  return transfer_;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMIDI::write( const Transfer& transfer_, uint8_t /* endpoint_ */)
{
  m_midiOut.sendMessage(const_cast<std::vector<unsigned char>*>(&(transfer_.getData())));
  return true;
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::readAsync(uint8_t /* endpoint_ */, DeviceHandleImpl::tCbRead cbRead_)
{
  m_cbRead = cbRead_;
  m_midiIn.setCallback(&DeviceHandleMIDI::onMidiMessage, this);
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::onMidiMessage(
  double timeStamp_,
  std::vector<unsigned char> *pMessage_,
  void *pUserData_
)
{
  if(nullptr == pMessage_ || nullptr == pUserData_)
  {
    return;
  }
  
  DeviceHandleMIDI* pSelf = static_cast<DeviceHandleMIDI*>(pUserData_);
  pSelf->m_cbRead(tRawData{pMessage_->begin(),pMessage_->end()});
}

//--------------------------------------------------------------------------------------------------

} // kio
} // sl
