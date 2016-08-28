/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "util/ColorRGB.h"
#include <cmath>

namespace sl
{
namespace util
{

//--------------------------------------------------------------------------------------------------

ColorRGB::ColorRGB(uint8_t mono_) : m_red(mono_), m_green(mono_), m_blue(mono_), m_mono(mono_)
{
}

//--------------------------------------------------------------------------------------------------

ColorRGB::ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_)
  : m_red(red_)
  , m_green(green_)
  , m_blue(blue_)
  , m_mono(util::max<uint8_t>(red_, green_, blue_)) // Max decomposition: take the highest value
{
}

//--------------------------------------------------------------------------------------------------

ColorRGB::ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_)
  : m_red(red_), m_green(green_), m_blue(blue_), m_mono(mono_)
{
}

//--------------------------------------------------------------------------------------------------

double ColorRGB::distance(const ColorRGB& other_) const
{
  return sqrt(std::pow(((m_red - other_.m_red) * 0.299), 2.0)
              + std::pow(((m_green - other_.m_green) * 0.587), 2.0)
              + std::pow(((m_blue - other_.m_blue) * 0.114), 2.0));
}

//--------------------------------------------------------------------------------------------------

unsigned ColorRGB::red() const
{
  return m_red;
}

//--------------------------------------------------------------------------------------------------

unsigned ColorRGB::green() const
{
  return m_green;
}

//--------------------------------------------------------------------------------------------------

unsigned ColorRGB::blue() const
{
  return m_blue;
}

//--------------------------------------------------------------------------------------------------

unsigned ColorRGB::mono() const
{
  return m_mono;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::setRed(uint8_t red_)
{
  m_red = red_;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::setGreen(uint8_t green_)
{
  m_green = green_;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::setBlue(uint8_t blue_)
{
  m_blue = blue_;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::setMono(uint8_t mono_)
{
  m_mono = mono_;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::black()
{
  m_red = m_blue = m_green = m_mono = 0;
}

//--------------------------------------------------------------------------------------------------

void ColorRGB::white()
{
  m_red = m_blue = m_green = m_mono = 0xFF;
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator==(const ColorRGB& other_) const
{
  return (m_red == other_.m_red) && (m_green == other_.m_green) && (m_blue == other_.m_blue);
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator!=(const ColorRGB& other_) const
{
  return !(operator==(other_));
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator<(const ColorRGB& other_) const
{
  return getValue() < other_.getValue();
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator<=(const ColorRGB& other_) const
{
  return getValue() <= other_.getValue();
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator>(const ColorRGB& other_) const
{
  return getValue() > other_.getValue();
}

//--------------------------------------------------------------------------------------------------

bool ColorRGB::operator>=(const ColorRGB& other_) const
{
  return getValue() >= other_.getValue();
}
//--------------------------------------------------------------------------------------------------

unsigned ColorRGB::getValue() const
{
  return (m_red << 16) | (m_green << 8) | m_blue;
}

//--------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out_, const ColorRGB& c_)
{
  out_ << static_cast<int>(c_.m_red) << "," << static_cast<int>(c_.m_green) << ","
       << static_cast<int>(c_.m_blue) << "," << static_cast<int>(c_.m_mono);
  return out_;
}

//--------------------------------------------------------------------------------------------------

} // namespace util
} // namespace sl
