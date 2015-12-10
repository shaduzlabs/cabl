/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "ColorDisplay.h"

#include <sstream>

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace midi;
using namespace util;
using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

ColorDisplay::ColorDisplay()
{
  m_client.setCallbacks(
    [this](){ initHardware(); },
    [this](){ tick(); },
    [this](){ discoverAndConnect(); }
  );
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::run()
{
  m_client.run();
  discoverAndConnect();
  while(true)
  {
  
  }
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::initHardware()
{
  m_client.getDevice()->getGraphicDisplay(0)->black();
  m_client.getDevice()->getGraphicDisplay(1)->black();
  
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::tick()
{
  if(m_update)
  {
    updateDisplay();
    m_update = false;
  }
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::discoverAndConnect()
{
  static unsigned retryDelayInSeconds(5);
  auto devices = Client::enumerateDevices();
  while(devices.size()<=0)
  {
    M_LOG("[Application] no devices found. Retrying in " << retryDelayInSeconds << " seconds" );
    std::this_thread::sleep_for(std::chrono::seconds(retryDelayInSeconds));
    devices = Client::enumerateDevices();
  }
  m_client.connect(devices[0]);
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::updateDisplay()
{
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
