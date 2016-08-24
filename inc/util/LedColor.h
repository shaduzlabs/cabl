/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Functions.h"
#include "RGBColor.h"

namespace sl
{
namespace util
{

class LedColor
{
public:
  LedColor(uint8_t mono_) : m_red(mono_), m_green(mono_), m_blue(mono_), m_mono(mono_)
  {
  }

  LedColor(uint8_t red_, uint8_t green_, uint8_t blue_)
    : m_red(red_)
    , m_green(green_)
    , m_blue(blue_)
    , m_mono(util::max<uint8_t>(red_, green_, blue_)) // Max decomposition: take the highest value
  {
  }

  LedColor(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_)
    : m_red(red_), m_green(green_), m_blue(blue_), m_mono(mono_)
  {
  }

  RGBColor colorRGB() const
  {
    return {
      static_cast<uint8_t>(m_red), static_cast<uint8_t>(m_green), static_cast<uint8_t>(m_blue)};
  }

  unsigned red() const
  {
    return m_red;
  }
  unsigned green() const
  {
    return m_green;
  }
  unsigned blue() const
  {
    return m_blue;
  }
  unsigned mono() const
  {
    return m_mono;
  }

  void setRed(uint8_t red_)
  {
    m_red = red_;
  }
  void setGreen(uint8_t green_)
  {
    m_green = green_;
  }
  void setBlue(uint8_t blue_)
  {
    m_blue = blue_;
  }
  void setMono(uint8_t mono_)
  {
    m_mono = mono_;
  }

  void black()
  {
    m_red = m_blue = m_green = m_mono = 0;
  }
  void white()
  {
    m_red = m_blue = m_green = m_mono = 0xFF;
  }

private:
  friend std::ostream& operator<<(std::ostream& out_, const LedColor& lc_)
  {
    out_ << lc_.m_red << "," << lc_.m_green << "," << lc_.m_blue << "," << lc_.m_mono;
    return out_;
  }

  unsigned m_red;
  unsigned m_green;
  unsigned m_blue;
  unsigned m_mono;
};

} // namespace util
} // namespace sl
