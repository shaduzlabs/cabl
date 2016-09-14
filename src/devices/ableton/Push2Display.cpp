/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ableton/Push2Display.h"
#include "cabl/comm/Driver.h"
#include "cabl/comm/Transfer.h"
#include "cabl/util/Functions.h"

#include <algorithm>
#include <thread>

#include "cabl/gfx/TextDisplay.h"
#include "gfx/displays/NullCanvas.h"

#include <cmath>

//!\todo delete debug includes
#include <iomanip>
#include <iostream>

namespace
{
const std::string kPush2_midiPortName = "Ableton Push 2 Live Port";
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{


Push2Display::Push2Display() : m_pMidiOut(new RtMidiOut), m_pMidiIn(new RtMidiIn)
{
  /*
    std::string portName;
    unsigned nPorts = m_pMidiOut->getPortCount();
    for (unsigned int i = 0; i < nPorts; i++)
    {
      try
      {
        portName = m_pMidiOut->getPortName(i);
        if (portName.find(kPush2_midiPortName) != std::string::npos)
        {
          m_pMidiOut->openPort(i);
        }
      }
      catch (RtMidiError& error)
      {
        M_LOG("[MaschineMK2] RtMidiError: " << error.message());
      }
    }
    if (!m_pMidiOut->isPortOpen())
    {
      m_pMidiOut.reset(nullptr);
    }

    portName.clear();
    nPorts = m_pMidiIn->getPortCount();
    for (unsigned int i = 0; i < nPorts; i++)
    {
      try
      {
        portName = m_pMidiIn->getPortName(i);
        if (portName.find(kPush2_midiPortName) != std::string::npos)
        {
          m_pMidiIn->openPort(i);
        }
      }
      catch (RtMidiError& error)
      {
        M_LOG("[MaschineMK2] RtMidiError: " << error.message());
      }
    }
    if (!m_pMidiIn->isPortOpen())
    {
      m_pMidiIn.reset(nullptr);
    }
    else
    {
      m_pMidiIn->setCallback(&Push2Display::midiInCallback, this);
    }
    */
}

//--------------------------------------------------------------------------------------------------

Push2Display::~Push2Display()
{
  M_LOG("[Push2Display]: destructor");
}

//--------------------------------------------------------------------------------------------------

Canvas* Push2Display::graphicDisplay(size_t displayIndex_)
{
  static NullCanvas s_dummyDisplay;
  if (displayIndex_ > 0)
  {
    return &s_dummyDisplay;
  }

  return &m_display;
}

//--------------------------------------------------------------------------------------------------

bool Push2Display::tick()
{
  if (m_display.dirty())
  {
    return sendDisplayData();
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

void Push2Display::init()
{
  // Leds
  m_display.black();
}

//--------------------------------------------------------------------------------------------------

bool Push2Display::sendDisplayData() const
{
  bool result = true;
  writeToDeviceHandle(
    Transfer(
      {0xEF, 0xCD, 0xAB, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}),
    0x01);

  for (unsigned offset = 0; offset < m_display.bufferSize(); offset += 16384)
  {
    if (!writeToDeviceHandle(
          Transfer({m_display.data() + offset, m_display.data() + offset + 16384}), 0x01))
    {
      return false;
    }
  }
  m_display.resetDirtyFlags();
  return result;
}

//--------------------------------------------------------------------------------------------------

void Push2Display::processNote(uint8_t note_, uint8_t velocity_)
{
  if (note_ <= 10)
  {
    // Touch encoders
    //   uint8_t offset = static_cast<uint8_t>(Button::TouchEncoder1);
    //   Device::Button btn = deviceButton(static_cast<Button>(note_+offset));
    //   buttonChanged(btn, (velocity_>0), m_shiftPressed);
  }
  else if (note_ == 12)
  {
    // Pitch bend
  }
  else if (note_ >= 36 && note_ <= 99)
  {
    // Pads
    Device::Pad pad = static_cast<Device::Pad>(note_ - 36);
    padChanged(pad, (velocity_ > 0), m_shiftPressed);
  }
}

//--------------------------------------------------------------------------------------------------

void Push2Display::midiInCallback(
  double timeStamp_, std::vector<unsigned char>* pMessage_, void* userData_)
{
  Push2Display* pSelf = static_cast<Push2Display*>(userData_);
  if ((pMessage_->at(0) & 0xf0) == 0x90)
  {
    pSelf->processNote(pMessage_->at(1), pMessage_->at(2));
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
