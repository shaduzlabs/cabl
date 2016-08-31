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

ColorDisplay::ColorDisplay(const std::string& pngFolder_) : m_pngFolder(pngFolder_)
{
  std::string pngFile = "1.png";
  std::cout << "pushER v.0.0.1 / shaduzLABS" << std::endl;
  std::cout << "Using PNG file: " << pngFile << std::endl;
  m_client.setCallbacks(
    [this]() { initHardware(); }, [this]() { tick(); }, [this]() { discoverAndConnect(); });

  tryLoadFile();
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::run()
{
  m_client.run();
  discoverAndConnect();
  while (true)
  {
    if (std::cin.get() == '\n')
    {
      tryLoadFile();
    }
  }
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::initHardware()
{
  m_pDevice->displayGraphic(0)->black();
  m_pDevice->displayGraphic(1)->black();
  m_pDevice->setCallbackPadChanged(std::bind(&ColorDisplay::padChanged, this, _1, _2, _3));

  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::onTick()
{
  updateDisplay();
  m_update = false;
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::discoverAndConnect()
{
  static unsigned retryDelayInSeconds(5);
  auto devices = Client::enumerateDevices();
  while (devices.size() <= 0)
  {
    M_LOG("[Application] no devices found. Retrying in " << retryDelayInSeconds << " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(retryDelayInSeconds));
    devices = Client::enumerateDevices();
  }
  m_client.connect(devices[0]);
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed_)
{
  static auto lastEvent = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  if (now - lastEvent > std::chrono::milliseconds(180))
  {
    lastEvent = now;
    if (m_nFile != static_cast<unsigned>(pad_))
    {
      m_nFile = static_cast<unsigned>(pad_);
      tryLoadFile();
      m_update = true;
    }
  }
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::tryLoadFile()
{
  m_pngImage.clear();
  std::string pngFile = m_pngFolder + "/" + std::to_string(m_nFile++) + ".png";
  unsigned error = lodepng::decode(m_pngImage, m_pngWidth, m_pngHeight, pngFile);

  if (error)
  {
    M_LOG("decoder error " << std::to_string(error) << ": " << lodepng_error_text(error));
    m_nFile = 1;
    tryLoadFile();
  }
  else
  {
    if (m_pngWidth != 1024)
    {
      throw std::runtime_error(
        "decoder error: image width must be 1024 pixels (current image width is "
        + std::to_string(m_pngWidth)
        + " pixels)");
    }
    if (m_pngHeight != 160)
    {
      throw std::runtime_error(
        "decoder error: image height must be 160 pixels (current image height is "
        + std::to_string(m_pngWidth)
        + " pixels)");
    }
    std::cout << "Loaded PNG file " << pngFile << " which is " << m_pngWidth << "x" << m_pngHeight
              << " pixels / " << m_pngImage.size() << " bytes" << std::endl;
  }
}

//--------------------------------------------------------------------------------------------------

void ColorDisplay::updateDisplay()
{
  static unsigned nTick = 0;
  m_update = true;
  nTick++;
  if (nTick > 2)
  {
    nTick = 0;

    uint8_t* pDrawingContext = m_pDevice->displayGraphic(0).data().data();
    uint8_t* pPNG = m_pngImage.data();
    for (unsigned col = 0; col < m_pngWidth; col++)
    {
      for (unsigned row = 0; row < m_pngHeight; row++)
      {
        uint8_t r = static_cast<uint8_t>(round(((float)*pPNG++ / 255.0) * 31.0));
        uint8_t g = static_cast<uint8_t>(round(((float)*pPNG++ / 255.0) * 63.0));
        uint8_t b = static_cast<uint8_t>(round(((float)*pPNG++ / 255.0) * 31.0));
        uint8_t alpha = *pPNG++;
        *pDrawingContext++ = (uint8_t)((g & 7) << 5) | (r & 0x1f);
        *pDrawingContext++ = ((b & 0x1f) << 3) | ((g & 0x38) >> 3);
      }
    }
    m_pDevice->drawingContext(0).setDirty(true);
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
