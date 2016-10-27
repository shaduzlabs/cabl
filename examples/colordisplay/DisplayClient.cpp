/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DisplayClient.h"

#include <algorithm>
#include <cmath>
#include <sstream>

#include <lodepng.h>

#include <cabl/gfx/Canvas.h>

namespace
{
}

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

DisplayClient::DisplayClient() : Client({"Ableton Push 2"})
{
}

//--------------------------------------------------------------------------------------------------

void DisplayClient::initDevice()
{

}

//--------------------------------------------------------------------------------------------------

void DisplayClient::render()
{
}

//--------------------------------------------------------------------------------------------------

void DisplayClient::buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_)
{
}

//--------------------------------------------------------------------------------------------------

void DisplayClient::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
}

//--------------------------------------------------------------------------------------------------

void DisplayClient::keyChanged(unsigned index_, double value_, bool shiftPressed)
{
}

//--------------------------------------------------------------------------------------------------

void DisplayClient::controlChanged(unsigned pot_, double value_, bool shiftPressed)
{
}

//--------------------------------------------------------------------------------------------------

bool DisplayClient::showPng(const std::string& pngFilePath_)
{
  std::vector<unsigned char> png;
  std::vector<unsigned char> pixels;
  unsigned width, height;

  // load and decode
  unsigned error = lodepng::load_file(png, pngFilePath_.c_str());
  if (!error)
  {
    error = lodepng::decode(pixels, width, height, png);
  }

  if (error)
  {
    std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return false;
  }

  for (unsigned y = 0; y < height; y++)
  {
    for (unsigned x = 0; x < width; x++)
    {
      unsigned index = 4 * ((y * width) + x);
      device()->graphicDisplay(0)->setPixel(
        x, y, {pixels[index], pixels[index + 1], pixels[index + 2]});
    }
  }
  
  return true;
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
