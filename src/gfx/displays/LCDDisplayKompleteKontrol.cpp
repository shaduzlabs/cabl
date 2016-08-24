/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/LCDDisplayKompleteKontrol.h"

#include <cmath>
#include <stdint.h>
#include <string>
#ifndef ARDUINO
#include <algorithm>
#endif

#include "util/Log.h"

namespace
{
static const uint8_t kLCDKK_numDotsPerRow = 7;
static const uint16_t kLCDDisplayKK_FontData[] = {
#include "gfx/fonts/data/FONT_16-seg.h"
};
}

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

LCDDisplayKompleteKontrol::LCDDisplayKompleteKontrol() : LCDDisplay(kLCDKK_numCols, kLCDKK_numRows)
{
  data().resize(48);
  for (uint8_t i = 0; i < kLCDKK_numRows; i++)
  {
    m_dirtyFlags[i] = false;
  }
}

//--------------------------------------------------------------------------------------------------

LCDDisplayKompleteKontrol::~LCDDisplayKompleteKontrol()
{
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::clear()
{
  data().clear();
  setDirty(true);
}

//--------------------------------------------------------------------------------------------------

bool LCDDisplayKompleteKontrol::isDirtyRow(uint8_t row_) const
{
  if (row_ >= kLCDKK_numRows)
  {
    return false;
  }
  return m_dirtyFlags[row_];
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setCharacter(uint8_t col_, uint8_t row_, char c_)
{
  if (row_ < 1 || row_ >= kLCDKK_numRows || col_ >= kLCDKK_numCols)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = (row_ * 16) + col_;
  data()[index++] = kLCDDisplayKK_FontData[static_cast<uint8_t>(c_)] & 0xff;
  data()[index++] = (kLCDDisplayKK_FontData[static_cast<uint8_t>(c_)] >> 8) & 0xff;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(const std::string& string_, uint8_t row_, Align align_)
{
  if (row_ == 0 || row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = row_ * 16;
  std::string strAligned = alignText(string_, align_);
  for (size_t i = 0; i < std::min<size_t>(strAligned.length(), 8); i++)
  {
    const uint8_t& character = strAligned.at(i);
    data()[index++] = kLCDDisplayKK_FontData[character] & 0xff;
    data()[index++] = (kLCDDisplayKK_FontData[character] >> 8) & 0xff;
  }
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(int value_, uint8_t row_, Align align_)
{
  setText(std::to_string(value_), row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(double value_, uint8_t row_, Align align_)
{
  double integral;
  double fractional = modf(value_, &integral);
  std::string strValue = std::to_string(static_cast<int>(integral));
  std::string strFractional = std::to_string(static_cast<int>(fractional * 1000));
  uint8_t emptySpaces = kLCDKK_numCols - strValue.length() - strFractional.length();
  uint8_t leftFills = static_cast<uint8_t>(emptySpaces / 2.0f);
  resetDots(row_);
  setDot(strValue.length() - 1 + leftFills, row_);
  strValue.append(std::string(3 - strFractional.length(), '0'));
  strValue.append(strFractional);

  setText(strValue, row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setValue(float value_, uint8_t row_, Align align_)
{
  if (row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;

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
        data()[index++] = kLCDDisplayKK_FontData[43] & 0xff;
        data()[index++] = (kLCDDisplayKK_FontData[43] >> 8) & 0xff;
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

std::string LCDDisplayKompleteKontrol::alignText(const std::string& string_, Align align_) const
{
  if (string_.length() >= kLCDKK_numCols)
  {
    return string_.substr(0, kLCDKK_numCols);
  }

  std::string strValue(string_);
  switch (align_)
  {
    case Align::Right:
    {
      strValue.insert(0, kLCDKK_numCols - strValue.length(), ' ');
      break;
    }
    case Align::Center:
    {
      uint8_t nFills = kLCDKK_numCols - strValue.length();
      uint8_t leftFills = static_cast<uint8_t>(nFills / 2.0f);
      strValue.insert(0, leftFills, ' ');
      strValue.append(nFills - leftFills, ' ');
      break;
    }
    case Align::Left:
    default:
    {
      strValue.append(kLCDKK_numCols - strValue.length(), ' ');
      break;
    }
  }
  return strValue;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setDot(uint8_t nDot_, uint8_t row_, bool visible_)
{
  if (row_ == 0 || row_ >= kLCDKK_numRows || nDot_ > kLCDKK_numDotsPerRow)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;

  uint8_t mask = 1 << (row_ - 1);
  data()[(2 * nDot_) + 1] |= mask;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::resetDots(uint8_t row_)
{
  if (row_ == 0 || row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;

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

} // cabl
} // sl
