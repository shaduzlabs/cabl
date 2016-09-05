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

namespace sl
{
namespace cabl
{
template <unsigned ROWS, unsigned COLUMNS>
class LCDDisplayGeneric : public LCDDisplayBase<ROWS, COLUMNS>
{

public:

  ~LCDDisplayGeneric() = default;

  void clear() override;

  void setCharacter(uint8_t col_, uint8_t row_, char c_) override;

  void setText(const std::string& string_, uint8_t row_, LCDDisplay::Align align_) override;

  void setText(int value_, uint8_t row_, LCDDisplay::Align align_) override;

  void setText(double value_, uint8_t row_, LCDDisplay::Align align_) override;

  void setValue(float value_, uint8_t row_, LCDDisplay::Align align_) override;

private:
  std::string alignText(const std::string&, LCDDisplay::Align align_) const;
};

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::clear()
{
  this->fill(0x20);
}

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::setCharacter(uint8_t col_, uint8_t row_, char c_)
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

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::setText(
  const std::string& string_, uint8_t row_, LCDDisplay::Align align_)
{
  if (row_ >= this->height())
  {
    return;
  }
  this->setDirty(row_);

  unsigned index = row_ * this->width();
  std::string strAligned = alignText(string_, align_);
  for (size_t i = 0; i < std::min<size_t>(strAligned.length(), this->width()); i++)
  {
    const uint8_t& character = strAligned.at(i);
    this->data()[index++] = character;
  }
}

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::setText(int value_, uint8_t row_, LCDDisplay::Align align_)
{
  setText(std::to_string(value_), row_, align_);
}

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::setText(
  double value_, uint8_t row_, LCDDisplay::Align align_)
{
  double integral;
  double fractional = modf(value_, &integral);
  std::string strValue = std::to_string(static_cast<int>(integral));
  std::string strFractional = std::to_string(static_cast<int>(fractional * 1000));

  strValue.append(".");
  strValue.append(std::string(3 - strFractional.length(), '0'));
  strValue.append(strFractional);

  setText(strValue, row_, align_);
}

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
void LCDDisplayGeneric<ROWS, COLUMNS>::setValue(
  float value_, uint8_t row_, LCDDisplay::Align align_)
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
      //   data()[index++] = kLCDDisplayKK_FontData[43] & 0xff;
      //   data()[index++] = (kLCDDisplayKK_FontData[43] >> 8) & 0xff;
    }
    else
    {
      this->data()[index++] = 0x20;
    }
  }
}

//--------------------------------------------------------------------------------------------------

template <unsigned ROWS, unsigned COLUMNS>
std::string LCDDisplayGeneric<ROWS, COLUMNS>::alignText(
  const std::string& string_, LCDDisplay::Align align_) const
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

} // namespace cabl
} // namespace sl
