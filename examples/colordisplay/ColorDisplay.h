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

#include "lodepng.h"

namespace sl
{

using namespace cabl;

class ColorDisplay 
{
public:

  ColorDisplay(const std::string& pngFile_);
  void run();

  void initHardware();
  void tick();
  void discoverAndConnect();

  void padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed);
  void tryLoadFile();

private:

  void updateDisplay();

  Client       m_client;

  std::atomic<bool> m_update;
  std::string m_pngFolder;
  
  std::vector<unsigned char> m_pngImage;
  unsigned m_pngWidth;
  unsigned m_pngHeight;
  unsigned m_nFile{1};

};

} // namespace sl
