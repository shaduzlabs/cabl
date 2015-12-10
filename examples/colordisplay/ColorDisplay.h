/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <future>
#include <stdint.h>

#include <cabl.h>

namespace sl
{

using namespace cabl;

class ColorDisplay
{
public:

  ColorDisplay();
  void run();
  
  void initHardware();
  void tick();
  void discoverAndConnect();
  
private:

  void updateDisplay();
  
  Client       m_client;
  
  std::atomic<bool> m_update;
  
};

} // namespace sl
