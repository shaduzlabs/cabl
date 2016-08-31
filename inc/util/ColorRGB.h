/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <sstream>

#include "Functions.h"

namespace sl
{
namespace util
{

class ColorRGB
{
public:
  enum class BlendMode
  {
    Normal,       // Normal mode
    Invert,       // Invert the current color
    Transparent,  // Ignore the current color
  };
  
  enum class Name
  {
    White,       // Normal mode
    Red,       // Invert the current color
    Transparent,  // Ignore the current color
  };
  
  ColorRGB();
  
  ColorRGB(BlendMode);

  ColorRGB(uint8_t mono_);

  ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_);

  ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_);

  double distance(const ColorRGB& other_) const;

  uint8_t red() const;
  uint8_t green() const;
  uint8_t blue() const;
  uint8_t mono() const;
  bool active() const;
  BlendMode blendMode() const;
  bool transparent() const;
  
  void setRed(uint8_t red_);
  void setGreen(uint8_t green_);
  void setBlue(uint8_t blue_);
  void setMono(uint8_t mono_);
  void setBlendMode(BlendMode blendMode_);

  void black();
  void white();
  void invert();
  
  bool operator==(const ColorRGB& other_) const;
  bool operator!=(const ColorRGB& other_) const;
  bool operator<(const ColorRGB& other_) const;
  bool operator<=(const ColorRGB& other_) const;
  bool operator>(const ColorRGB& other_) const;
  bool operator>=(const ColorRGB& other_) const;

  unsigned getValue() const;

  
private:
  friend std::ostream& operator<<(std::ostream& out_, const ColorRGB& c_);
  
  uint8_t m_red{0U};
  uint8_t m_green{0U};
  uint8_t m_blue{0U};
  uint8_t m_mono{0U};
  BlendMode m_blendMode{BlendMode::Normal};
};

} // namespace util
} // namespace sl
