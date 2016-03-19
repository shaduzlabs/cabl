/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Functions.h"
#include <cmath>
#include <iomanip>

namespace sl
{
namespace util
{

class RGBColor
{
public:
  RGBColor(uint8_t r_, uint8_t g_, uint8_t b_)
  :m_red(r_)
  ,m_green(g_)
  ,m_blue(b_)
  {}

  double distance(const RGBColor& other_) const
  {
    return sqrt(
      pow(((m_red   - other_.m_red  ) * 0.299), 2.0) +
      pow(((m_green - other_.m_green) * 0.587), 2.0) +
      pow(((m_blue  - other_.m_blue ) * 0.114), 2.0)
    );
  }
  
  bool operator==(const RGBColor& other_) const
  {
    return (m_red == other_.m_red) && (m_green == other_.m_green) && (m_blue == other_.m_blue);
  }

  bool operator!=(const RGBColor& other_) const
  {
    return !(operator==(other_));
  }

  bool operator<(const RGBColor& other_) const
  {
    return getValue() < other_.getValue();
  }  

  unsigned getValue() const
  {
    return (m_red << 16) | (m_green << 8) | m_blue;
  }
  
private:

  friend std::ostream& operator<<(std::ostream& out_, const RGBColor& lc_)
  {
    out_ << std::hex << std::setw(6) << lc_.getValue() << std::dec;
    return out_;
  }
  
  uint8_t m_red;
  uint8_t m_green;
  uint8_t m_blue;
};

} // namespace util
} // namespace sl
