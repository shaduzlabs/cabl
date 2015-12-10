/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ableton/Push2Display.h"
#include "comm/Driver.h"
#include "comm/Transfer.h"
#include "util/Functions.h"

#include <algorithm>
#include <thread>

#include "gfx/LCDDisplay.h"
#include "gfx/displays/GDisplayDummy.h"

#include <cmath>

//!\todo delete debug includes
#include <iostream>
#include <iomanip>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

GDisplay* Push2Display::getGraphicDisplay(uint8_t displayIndex_)
{
  static GDisplayDummy s_dummyDisplay;
  return &s_dummyDisplay;
}

//--------------------------------------------------------------------------------------------------

LCDDisplay* Push2Display::getLCDDisplay(uint8_t displayIndex_)
{
  static LCDDisplay s_dummyLCDDisplay(0, 0);
  return &s_dummyLCDDisplay;
}

//--------------------------------------------------------------------------------------------------

bool Push2Display::tick()
{
  if(m_display.isDirty())
  {
    return sendDisplayData();
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

void Push2Display::init()
{
  // Leds
  m_display.clear();
}

//--------------------------------------------------------------------------------------------------

bool Push2Display::sendDisplayData()
{
  bool result = true;
  getDeviceHandle()->write(Transfer({0xEF,0xCD,0xAB,0x89,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}),0x01);
  
  for(unsigned offset = 0; offset < m_display.getSize(); offset+=16384)
  {
    if(!getDeviceHandle()->write(Transfer({m_display.getData().begin() + offset, m_display.getData().begin() + offset + 16384}),0x01))
    {
      return false;
    }
  }
  m_display.setDirty(false);
  return result;
}

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
