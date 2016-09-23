/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/util/Color.h"
#include <cmath>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

Color::Color() : m_blendMode(BlendMode::Transparent)
{
}

//--------------------------------------------------------------------------------------------------

Color::Color(BlendMode blendMode_) : m_blendMode(blendMode_)
{
}

//--------------------------------------------------------------------------------------------------

Color::Color(uint8_t mono_) : m_red(mono_), m_green(mono_), m_blue(mono_), m_mono(mono_)
{
}

//--------------------------------------------------------------------------------------------------

Color::Color(uint8_t red_, uint8_t green_, uint8_t blue_)
  : m_red(red_)
  , m_green(green_)
  , m_blue(blue_)
  , m_mono(max<uint8_t>(red_, green_, blue_)) // Max decomposition: take the highest value
{
}

//--------------------------------------------------------------------------------------------------

Color::Color(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_)
  : m_red(red_), m_green(green_), m_blue(blue_), m_mono(mono_)
{
}

//--------------------------------------------------------------------------------------------------

double Color::distance(const Color& other_) const
{
  return sqrt(std::pow(((red() - other_.red()) * 0.299), 2.0)
              + std::pow(((green() - other_.green()) * 0.587), 2.0)
              + std::pow(((blue() - other_.blue()) * 0.114), 2.0));
}

//--------------------------------------------------------------------------------------------------

uint8_t Color::red() const
{
  return (m_blendMode == BlendMode::Invert) ? ~m_red : m_red;
}

//--------------------------------------------------------------------------------------------------

uint8_t Color::green() const
{
  return (m_blendMode == BlendMode::Invert) ? ~m_green : m_green;
}

//--------------------------------------------------------------------------------------------------

uint8_t Color::blue() const
{
  return (m_blendMode == BlendMode::Invert) ? ~m_blue : m_blue;
}

//--------------------------------------------------------------------------------------------------

uint8_t Color::mono() const
{
  return (m_blendMode == BlendMode::Invert) ? ~m_mono : m_mono;
}

//--------------------------------------------------------------------------------------------------

bool Color::active() const
{
  return mono() > 127;
}
//--------------------------------------------------------------------------------------------------

BlendMode Color::blendMode() const
{
  return m_blendMode;
}
//--------------------------------------------------------------------------------------------------

bool Color::transparent() const
{
  return m_blendMode == BlendMode::Transparent;
}

//--------------------------------------------------------------------------------------------------

void Color::setRed(uint8_t red_)
{
  m_red = red_;
}

//--------------------------------------------------------------------------------------------------

void Color::setGreen(uint8_t green_)
{
  m_green = green_;
}

//--------------------------------------------------------------------------------------------------

void Color::setBlue(uint8_t blue_)
{
  m_blue = blue_;
}

//--------------------------------------------------------------------------------------------------

void Color::setMono(uint8_t mono_)
{
  m_mono = mono_;
}

//--------------------------------------------------------------------------------------------------

void Color::setBlendMode(BlendMode blendMode_)
{
  m_blendMode = blendMode_;
}

//--------------------------------------------------------------------------------------------------

void Color::black()
{
  m_red = m_blue = m_green = m_mono = 0;
  m_blendMode = BlendMode::Normal;
}

//--------------------------------------------------------------------------------------------------

void Color::white()
{
  m_red = m_blue = m_green = m_mono = 0xFF;
  m_blendMode = BlendMode::Normal;
}

//--------------------------------------------------------------------------------------------------

void Color::invert()
{
  m_blendMode = BlendMode::Invert;
}

//--------------------------------------------------------------------------------------------------

bool Color::operator==(const Color& other_) const
{
  return (m_red == other_.m_red) && (m_green == other_.m_green) && (m_blue == other_.m_blue)
         && (m_mono == other_.m_mono) && (m_blendMode == other_.m_blendMode);
}

//--------------------------------------------------------------------------------------------------

bool Color::operator!=(const Color& other_) const
{
  return !(operator==(other_));
}

//--------------------------------------------------------------------------------------------------

bool Color::operator<(const Color& other_) const
{
  return getValue() < other_.getValue() && m_mono < other_.m_mono
         && m_blendMode <= other_.m_blendMode;
}

//--------------------------------------------------------------------------------------------------

bool Color::operator<=(const Color& other_) const
{
  return getValue() <= other_.getValue() && m_mono <= other_.m_mono
         && m_blendMode <= other_.m_blendMode;
}

//--------------------------------------------------------------------------------------------------

bool Color::operator>(const Color& other_) const
{
  return getValue() > other_.getValue() && m_mono > other_.m_mono
         && m_blendMode >= other_.m_blendMode;
}

//--------------------------------------------------------------------------------------------------

bool Color::operator>=(const Color& other_) const
{
  return getValue() >= other_.getValue() && m_mono >= other_.m_mono
         && m_blendMode >= other_.m_blendMode;
}

//--------------------------------------------------------------------------------------------------

unsigned Color::getValue() const
{
  return (m_red << 16) | (m_green << 8) | m_blue;
}

//--------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out_, const Color& c_)
{
  out_ << static_cast<int>(c_.m_red) << "," << static_cast<int>(c_.m_green) << ","
       << static_cast<int>(c_.m_blue) << "," << static_cast<int>(c_.m_mono);
  return out_;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
