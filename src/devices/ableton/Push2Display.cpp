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
const sl::cabl::Transfer k_frameHeader(
  {0xEF, 0xCD, 0xAB, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
const unsigned k_singleTransferSize = 16384;
} // namespace

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

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
  return sendDisplayData();


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
  writeToDeviceHandle(k_frameHeader, 0x01);

  for (unsigned offset = 0; offset < m_display.bufferSize(); offset += k_singleTransferSize)
  {
    auto displayData = Transfer({m_display.data() + offset, m_display.data() + offset + k_singleTransferSize});
  /*  for(unsigned i = 0; i < displayData.size(); i+=4)
    {
      displayData[i] ^= 0xf3;
      displayData[i+1] ^= 0xe7;
      displayData[i+2] ^= 0xff;
      displayData[i+3] ^= 0xe7;
    }*/
    if (!writeToDeviceHandle(displayData, 0x01))
    {
      return false;
    }
  }
  m_display.resetDirtyFlags();
  return result;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
