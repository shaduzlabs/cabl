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

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
class TextDisplayGeneric : public TextDisplayBase<ROWS, COLUMNS>
{

public:
  ~TextDisplayGeneric() = default;

  //--------------------------------------------------------------------------------------------------

  void clear() override
  {
    this->fill(0x20);
  }

  //--------------------------------------------------------------------------------------------------

  void putCharacter(unsigned col_, unsigned row_, char c_) override
  {
    if (row_ < 1 || row_ >= this->height() || col_ >= this->width())
    {
      return;
    }
    this->setDirty(row_);
    unsigned index = (row_ * this->width()) + col_;
    this->data()[index++] = c_;
  }

  //--------------------------------------------------------------------------------------------------

  void putText(const std::string& string_, unsigned row_, Alignment align_) override
  {
    if (row_ >= this->height())
    {
      return;
    }
    this->setDirty(row_);

    unsigned index = row_ * this->width();
    std::string strAligned = alignText(string_, align_);
    for (unsigned i = 0; i < std::min<unsigned>(strAligned.length(), this->width()); i++)
    {
      const uint8_t& character = strAligned.at(i);
      this->data()[index++] = character;
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
    std::string strFractional = std::to_string(static_cast<int>(fractional * 1000));

    strValue.append(".");
    strValue.append(std::string(3 - strFractional.length(), '0'));
    strValue.append(strFractional);

    putText(strValue, row_, align_);
  }

  //--------------------------------------------------------------------------------------------------

  void putValue(float value_, unsigned row_, Alignment align_) override
  {
    if (row_ >= this->height())
    {
      return;
    }
    this->setDirty(row_);

    unsigned index = row_ * 16;
    float val = std::min<float>(value_, 1.0f);

    uint8_t valInterval = static_cast<uint8_t>(round(val * 8.0));
    for (uint8_t i = 0; i < 8; i++)
    {
      if (valInterval > i)
      {
        //   data()[index++] = kTextDisplayKK_FontData[43] & 0xff;
        //   data()[index++] = (kTextDisplayKK_FontData[43] >> 8) & 0xff;
      }
      else
      {
        this->data()[index++] = 0x20;
      }
    }
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
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
