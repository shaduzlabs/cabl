/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/TextDisplayKompleteKontrol.h"

#include <cmath>
#include <cstdint>
#include <string>
#ifndef ARDUINO
#include <algorithm>
#endif

#include "cabl/util/Log.h"

namespace
{
const uint8_t kLCDKK_numDotsPerRow = 7;
const unsigned kTextDisplayKK_FontData[] = {
#include "gfx/fonts/data/FONT_16-seg.h"
};
} // namespace

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::putCharacter(unsigned col_, unsigned row_, char c_)
{
  if (row_ < 1 || row_ >= height() || col_ >= width())
  {
    return;
  }
  setDirty(row_);
  unsigned index = (row_ * 16) + col_;
  data()[index++] = kTextDisplayKK_FontData[static_cast<uint8_t>(c_)] & 0xff;
  data()[index++] = (kTextDisplayKK_FontData[static_cast<uint8_t>(c_)] >> 8) & 0xff;
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::putText(const std::string& string_, unsigned row_, Alignment align_)
{
  if (row_ == 0 || row_ >= height())
  {
    return;
  }
  setDirty(row_);

  unsigned index = row_ * 16;
  std::string strAligned = alignText(string_, align_);
  for (size_t i = 0; i < std::min<size_t>(strAligned.length(), 8); i++)
  {
    const uint8_t& character = strAligned.at(i);
    data()[index++] = kTextDisplayKK_FontData[character] & 0xff;
    data()[index++] = (kTextDisplayKK_FontData[character] >> 8) & 0xff;
  }
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::putText(int value_, unsigned row_, Alignment align_)
{
  if (row_ == 0 || row_ >= height())
  {
    return;
  }

  putText(std::to_string(value_), row_, align_);
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::putText(double value_, unsigned row_, Alignment align_)
{
  if (row_ == 0 || row_ >= height())
  {
    return;
  }

  double integral;
  double fractional = modf(value_, &integral);
  std::string strValue = std::to_string(static_cast<int>(integral));
  std::string strFractional = std::to_string(static_cast<int>(fractional * 1000));
  unsigned emptySpaces = width() - strValue.length() - strFractional.length();
  unsigned leftFills = static_cast<unsigned>(emptySpaces / 2.0f);
  resetDots(row_);
  setDot(strValue.length() - 1 + leftFills, row_);
  strValue.append(std::string(3 - strFractional.length(), '0'));
  strValue.append(strFractional);

  putText(strValue, row_, align_);
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::putValue(float value_, unsigned row_, Alignment align_)
{
  if (row_ >= height())
  {
    return;
  }
  setDirty(row_);

  unsigned index = row_ * 16;
  float val = std::min<float>(value_, 1.0f);

  if (row_ == 0)
  {
    uint8_t valInterval = static_cast<uint8_t>(round(val * 9.0));
    data()[0] = 0x04 | (valInterval > 0 ? 0x03 : 0x00); // 1st bar + surrounding block (3rd bit)
    data()[2] = valInterval > 1 ? 0x03 : 0x00;
    data()[4] = valInterval > 2 ? 0x03 : 0x00;
    data()[6] = valInterval > 3 ? 0x03 : 0x00;
    data()[8] = valInterval > 4 ? 0x03 : 0x00;
    data()[10] = valInterval > 5 ? 0x03 : 0x00;
    data()[12] = valInterval > 6 ? 0x03 : 0x00;
    data()[14] = valInterval > 7 ? 0x03 : 0x00;
    data()[15] = valInterval > 8 ? 0x03 : 0x00;
  }
  else
  {
    uint8_t valInterval = static_cast<uint8_t>(round(val * 8.0));
    for (uint8_t i = 0; i < 8; i++)
    {
      if (valInterval > i)
      {
        data()[index++] = kTextDisplayKK_FontData[43] & 0xff;
        data()[index++] = (kTextDisplayKK_FontData[43] >> 8) & 0xff;
      }
      else
      {
        data()[index++] = 0;
        data()[index++] = 0;
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

std::string TextDisplayKompleteKontrol::alignText(
  const std::string& string_, Alignment align_) const
{
  if (string_.length() >= width())
  {
    return string_.substr(0, width());
  }

  std::string strValue(string_);
  switch (align_)
  {
    case Alignment::Right:
    {
      strValue.insert(0, width() - strValue.length(), ' ');
      break;
    }
    case Alignment::Center:
    {
      unsigned nFills = width() - strValue.length();
      uint8_t leftFills = static_cast<uint8_t>(nFills / 2.0f);
      strValue.insert(0, leftFills, ' ');
      strValue.append(nFills - leftFills, ' ');
      break;
    }
    case Alignment::Left:
    default:
    {
      strValue.append(width() - strValue.length(), ' ');
      break;
    }
  }
  return strValue;
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::setDot(unsigned nDot_, unsigned row_, bool visible_)
{
  if (row_ == 0 || row_ >= height() || nDot_ > kLCDKK_numDotsPerRow)
  {
    return;
  }
  setDirty(row_);

  uint8_t mask = 1 << (row_ - 1);
  data()[(2 * nDot_) + 1] |= mask;
}

//--------------------------------------------------------------------------------------------------

void TextDisplayKompleteKontrol::resetDots(unsigned row_)
{
  if (row_ == 0 || row_ >= height())
  {
    return;
  }
  setDirty(row_);

  uint8_t mask = 1 << (row_ - 1);

  data()[1] &= ~mask;
  data()[3] &= ~mask;
  data()[5] &= ~mask;
  data()[7] &= ~mask;
  data()[9] &= ~mask;
  data()[11] &= ~mask;
  data()[13] &= ~mask;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
