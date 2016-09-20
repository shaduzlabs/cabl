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

#include "cabl/gfx/TextDisplay.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

namespace detail
{
const uint8_t kTextDisplay7S_FontData[] = {
#include "gfx/fonts/data/FONT_7-seg.h"
};
} // namespace

//--------------------------------------------------------------------------------------------------

template <unsigned COLUMNS>
class TextDisplay7Segments : public TextDisplayBase<COLUMNS, 1>
{

public:
  ~TextDisplay7Segments() = default;

  //--------------------------------------------------------------------------------------------------

  void putCharacter(unsigned col_, unsigned row_, char c_) override
  {
    uint8_t charNum = static_cast<uint8_t>(c_);
    if (row_ > 0 || col_ > this->width() || charNum < 45 || charNum > 90)
    {
      return;
    }
    this->setDirty(0);
    this->data()[col_] = detail::kTextDisplay7S_FontData[charNum - 45];
  }

  //--------------------------------------------------------------------------------------------------

  void putText(const std::string& string_, unsigned row_, Alignment align_) override
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
      this->data()[i] = (character < 45 || character > 90)
                          ? 0x00
                          : detail::kTextDisplay7S_FontData[character - 45];
    }
  }

  //--------------------------------------------------------------------------------------------------

  void putText(int value_, unsigned row_, Alignment align_) override
  {
    putText(std::to_string(value_), row_, align_);
  }

  //--------------------------------------------------------------------------------------------------

  void putText(double value_, unsigned row_, Alignment align_) override
  {
    double integral;
    double fractional = modf(value_, &integral);
    std::string strValue = std::to_string(static_cast<int>(integral));
    std::string strFractional = std::to_string(static_cast<int>(fractional * 10));
    unsigned emptySpaces = this->width() - strValue.length() - strFractional.length();
	unsigned leftFills = static_cast<unsigned>(emptySpaces / 2.0f);
    resetDots(row_);
    setDot(strValue.length() - 1 + leftFills, row_);
    strValue.append(strFractional);

    putText(strValue, row_, align_);
  }

  //--------------------------------------------------------------------------------------------------

  void putValue(float value_, unsigned row_, Alignment align_) override
  {
    return;
  }

  //--------------------------------------------------------------------------------------------------

private:
  //--------------------------------------------------------------------------------------------------

  std::string alignText(const std::string& string_, Alignment align_) const
  {
    if (string_.length() >= this->width())
    {
      return string_.substr(0, this->width());
    }

    std::string strValue(string_);
    switch (align_)
    {
      case Alignment::Right:
      {
        strValue.insert(0, this->width() - strValue.length(), ' ');
        break;
      }
      case Alignment::Center:
      {
        unsigned nFills = this->width() - strValue.length();
		unsigned leftFills = static_cast<unsigned>(nFills / 2.0f);
        strValue.insert(0, leftFills, ' ');
        strValue.append(nFills - leftFills, ' ');
        break;
      }
      case Alignment::Left:
      default:
      {
        strValue.append(this->width() - strValue.length(), ' ');
        break;
      }
    }
    return strValue;
  }

  //--------------------------------------------------------------------------------------------------

  void setDot(unsigned nDot_, unsigned row_, bool visible_ = true)
  {
    if (row_ > 0 || nDot_ >= this->width())
    {
      return;
    }
    this->setDirty(0);
    this->data()[nDot_] |= 0x01;
  }

  //--------------------------------------------------------------------------------------------------

  void resetDots(unsigned row_)
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
