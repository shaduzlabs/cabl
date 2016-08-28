/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/drivers/MIDI/DeviceHandleMIDI.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

DeviceHandleMIDI::DeviceHandleMIDI(const DeviceDescriptor& deviceDescriptor_)
{
  try
  {
    m_midiIn.openPort(deviceDescriptor_.portIdIn(), deviceDescriptor_.name());
    m_midiOut.openPort(deviceDescriptor_.portIdOut(), deviceDescriptor_.name());
  }
  catch (RtMidiError e)
  {
  }
}

//--------------------------------------------------------------------------------------------------

DeviceHandleMIDI::~DeviceHandleMIDI()
{
  disconnect();
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::disconnect()
{
  try
  {
    m_midiIn.closePort();
    m_midiOut.closePort();
  }
  catch (RtMidiError e)
  {
  }
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMIDI::read(Transfer& transfer_, uint8_t /* endpoint_ */)
{
  m_midiIn.getMessage(const_cast<std::vector<unsigned char>*>(&(transfer_.data())));
  return transfer_;
}

//--------------------------------------------------------------------------------------------------

bool DeviceHandleMIDI::write(const Transfer& transfer_, uint8_t /* endpoint_ */)
{
  try
  {
    m_midiOut.sendMessage(const_cast<std::vector<unsigned char>*>(&(transfer_.data())));
  }
  catch (RtMidiError)
  {
    return false;
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::readAsync(uint8_t /* endpoint_ */, DeviceHandle::tCbRead cbRead_)
{
  m_cbRead = cbRead_;
  m_midiIn.setCallback(&DeviceHandleMIDI::onMidiMessage, this);
}

//--------------------------------------------------------------------------------------------------

void DeviceHandleMIDI::onMidiMessage(
  double timeStamp_, std::vector<unsigned char>* pMessage_, void* pUserData_)
{
  if (nullptr == pMessage_ || nullptr == pUserData_)
  {
    return;
  }

  DeviceHandleMIDI* pSelf = static_cast<DeviceHandleMIDI*>(pUserData_);
  pSelf->m_cbRead(tRawData{pMessage_->begin(), pMessage_->end()});
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
