/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#ifndef ARDUINO
#include <algorithm>
#endif

#include "gfx/LCDDisplay.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

namespace detail
{
const uint8_t kLCDDisplay7S_FontData[] = {
#include "gfx/fonts/data/FONT_7-seg.h"
};
} // namespace

//--------------------------------------------------------------------------------------------------

template <unsigned COLUMNS>
class LCDDisplay7Segments : public LCDDisplayBase<COLUMNS, 1>
{

public:

  ~LCDDisplay7Segments() = default;

//--------------------------------------------------------------------------------------------------

  void character(uint8_t col_, uint8_t row_, char c_) override
  {
    uint8_t charNum = static_cast<uint8_t>(c_);
    if (row_ > 0 || col_ > this->width() || charNum < 45 || charNum > 90)
    {
      return;
    }
    this->setDirty(0);
    this->data()[col_] = detail::kLCDDisplay7S_FontData[charNum - 45];
  }
  
//--------------------------------------------------------------------------------------------------

  void text(const std::string& string_, uint8_t row_, LCDDisplay::Align align_) override
  {
    if (row_ > 0)
    {
      return;
    }
    this->setDirty(0);

    std::string strAligned = alignText(string_, align_);
    std::transform(strAligned.begin(), strAligned.end(), strAligned.begin(), ::toupper);

    for (size_t i = 0; i < std::min<size_t>(strAligned.length(), this->width()); i++)
    {
      const uint8_t& character = strAligned.at(i);
      this->data()[i]
        = (character < 45 || character > 90) ? 0x00 : detail::kLCDDisplay7S_FontData[character - 45];
    }
  }
  
//--------------------------------------------------------------------------------------------------

  void text(int value_, uint8_t row_, LCDDisplay::Align align_) override
  {
    setText(std::to_string(value_), row_, align_);
  }
  
//--------------------------------------------------------------------------------------------------

  void text(double value_, uint8_t row_, LCDDisplay::Align align_) override
  {
    double integral;
    double fractional = modf(value_, &integral);
    std::string strValue = std::to_string(static_cast<int>(integral));
    std::string strFractional = std::to_string(static_cast<int>(fractional * 10));
    uint8_t emptySpaces = this->width() - strValue.length() - strFractional.length();
    uint8_t leftFills = static_cast<uint8_t>(emptySpaces / 2.0f);
    resetDots(row_);
    setDot(strValue.length() - 1 + leftFills, row_);
    strValue.append(strFractional);

    setText(strValue, row_, align_);
  }
  
//--------------------------------------------------------------------------------------------------

  void value(float value_, uint8_t row_, LCDDisplay::Align align_) override
  {
    return;
  }
  
//--------------------------------------------------------------------------------------------------

private:

//--------------------------------------------------------------------------------------------------

  std::string alignText(const std::string& string_, LCDDisplay::Align align_) const
  {
    if (string_.length() >= this->width())
    {
      return string_.substr(0, this->width());
    }

    std::string strValue(string_);
    switch (align_)
    {
      case LCDDisplay::Align::Right:
      {
        strValue.insert(0, this->width() - strValue.length(), ' ');
        break;
      }
      case LCDDisplay::Align::Center:
      {
        uint8_t nFills = this->width() - strValue.length();
        uint8_t leftFills = static_cast<uint8_t>(nFills / 2.0f);
        strValue.insert(0, leftFills, ' ');
        strValue.append(nFills - leftFills, ' ');
        break;
      }
      case LCDDisplay::Align::Left:
      default:
      {
        strValue.append(this->width() - strValue.length(), ' ');
        break;
      }
    }
    return strValue;
  }
  
//--------------------------------------------------------------------------------------------------

  void setDot(uint8_t nDot_, uint8_t row_, bool visible_ = true)
  {
    if (row_ > 0 || nDot_ >= this->width())
    {
      return;
    }
    this->setDirty(0);
    this->data()[nDot_] |= 0x01;
  }
  
//--------------------------------------------------------------------------------------------------

  void resetDots(uint8_t row_)
  {
    if (row_ > 0)
    {
      return;
    }
    this->setDirty(0);

    for (uint8_t i = 0; i < this->width(); i++)
    {
      this->data()[i] &= 0xfe;
    }
  }

//--------------------------------------------------------------------------------------------------

};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
