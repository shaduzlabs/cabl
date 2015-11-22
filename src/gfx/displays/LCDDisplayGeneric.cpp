/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/LCDDisplayGeneric.h"

#include <cmath>
#include <string>
#include <stdint.h>
#ifndef ARDUINO
#include <algorithm>
#endif

#include "util/Log.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------
 
LCDDisplayGeneric::LCDDisplayGeneric()
  : LCDDisplay(kLCDKK_numCols,kLCDKK_numRows)
{
  data().resize(kLCDKK_numRows*kLCDKK_numCols);
  clear();
}
  
//--------------------------------------------------------------------------------------------------

LCDDisplayGeneric::~LCDDisplayGeneric()
{
  
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::clear()
{
  for(auto& dataByte : data())
  {
    dataByte = 0x20;
  }
  setDirty(true);
}

//--------------------------------------------------------------------------------------------------

bool LCDDisplayGeneric::isDirtyRow(uint8_t row_) const
{
  if(row_ >= kLCDKK_numRows)
  {
    return false;
  }
  return m_dirtyFlags[row_];
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::setCharacter(uint8_t col_, uint8_t row_, char c_)
{
  if(row_<1 || row_>=kLCDKK_numRows || col_ >= kLCDKK_numCols)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = (row_ * kLCDKK_numCols) +col_;
  data()[index++] = c_;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::setText(const std::string& string_, uint8_t row_, Align align_)
{
  if(row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = row_ * kLCDKK_numCols;
  std::string strAligned = alignText(string_, align_);
  for(size_t i = 0; i < std::min<size_t>(strAligned.length(),kLCDKK_numCols);i++)
  {
    const uint8_t& character = strAligned.at(i);
    data()[index++] = character;
  }
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::setText(int value_, uint8_t row_, Align align_)
{
  setText(std::to_string(value_),row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::setText(double value_, uint8_t row_, Align align_)
{
  double integral;
  double fractional = modf(value_, &integral);
  std::string strValue = std::to_string(static_cast<int>(integral));
  std::string strFractional = std::to_string(static_cast<int>(fractional*1000));

  strValue.append(".");
  strValue.append(std::string( 3-strFractional.length(), '0'));
  strValue.append(strFractional);
  
  setText(strValue, row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayGeneric::setValue(float value_, uint8_t row_, Align align_)
{
  if(row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;

  unsigned index = row_ * 16;
  float val = std::min<float>(value_,1.0f);
  
  uint8_t valInterval = static_cast<uint8_t>(round(val*8.0));
  for(uint8_t i = 0; i<8;i++)
  {
    if(valInterval>i)
    {
   //   data()[index++] = kLCDDisplayKK_FontData[43] & 0xff;
   //   data()[index++] = (kLCDDisplayKK_FontData[43] >> 8) & 0xff;
    }
    else{
      data()[index++] = 0x20;
    }
  }
}

//--------------------------------------------------------------------------------------------------

std::string LCDDisplayGeneric::alignText(const std::string& string_, Align align_) const
{
  if(string_.length()>=kLCDKK_numCols)
  {
    return string_.substr(0,kLCDKK_numCols);
  }
  
  std::string strValue(string_);
  switch(align_)
  {
    case Align::Right:
    {
      strValue.insert(0, kLCDKK_numCols-strValue.length(),' ');
      break;
    }
    case Align::Center:
    {
      uint8_t nFills = kLCDKK_numCols-strValue.length();
      uint8_t leftFills = static_cast<uint8_t>(nFills / 2.0f);
      strValue.insert(0, leftFills,' ');
      strValue.append(nFills-leftFills,' ');
      break;
    }
    case Align::Left:
    default:
    {
      strValue.append(kLCDKK_numCols-strValue.length(), ' ');
      break;
    }
  }
  return strValue;
}

//--------------------------------------------------------------------------------------------------
  
} // cabl
} // sl
