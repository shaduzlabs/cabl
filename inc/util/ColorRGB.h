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
  ColorRGB(uint8_t mono_ = 0U);

  ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_);

  ColorRGB(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t mono_);

  double distance(const ColorRGB& other_) const;

  unsigned red() const;
  unsigned green() const;
  unsigned blue() const;
  unsigned mono() const;

  void setRed(uint8_t red_);
  void setGreen(uint8_t green_);
  void setBlue(uint8_t blue_);
  void setMono(uint8_t mono_);

  void black();
  void white();

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
};

} // namespace util
} // namespace sl
