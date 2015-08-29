/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/LCDDisplay7Segments.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <stdint.h>

#include "util/Log.h"

namespace
{
  static const uint8_t kLCDDisplay7S_FontData[] =
  {
#include "../fonts/data/FONT_7-seg.h"
  };
}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------
 
LCDDisplay7Segments::LCDDisplay7Segments(uint8_t numCharacters_)
  : LCDDisplay(numCharacters_,1)
{
  data().resize(numCharacters_);
}
  
//--------------------------------------------------------------------------------------------------

LCDDisplay7Segments::~LCDDisplay7Segments()
{
  
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::clear()
{
  data().clear();
  setDirty(true);
}

//--------------------------------------------------------------------------------------------------

bool LCDDisplay7Segments::isDirtyRow(uint8_t row_) const
{
  return isDirty();
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setCharacter(uint8_t col_, uint8_t row_, char c_)
{
  uint8_t charNum = static_cast<uint8_t>(c_);
  if(row_>0 || col_ > getNumberOfCharsPerRow() || charNum < 45 || charNum > 90)
  {
    return;
  }
  setDirty(true);
  data()[col_] = kLCDDisplay7S_FontData[charNum-45];
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setText(const std::string& string_, uint8_t row_, Align align_)
{
  if(row_>0)
  {
    return;
  }
  setDirty(true);

  std::string strAligned = alignText(string_, align_);
  std::transform(strAligned.begin(), strAligned.end(),strAligned.begin(), ::toupper);

  for(size_t i = 0; i < std::min<size_t>(strAligned.length(),getNumberOfCharsPerRow());i++)
  {
    const uint8_t& character = strAligned.at(i);
    data()[i] = (character < 45 && character > 90) ? 0x00 : kLCDDisplay7S_FontData[character-45];
  }
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setText(int value_, uint8_t row_, Align align_)
{
  setText(std::to_string(value_),row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setText(double value_, uint8_t row_, Align align_)
{
  double integral;
  double fractional = modf(value_, &integral);
  std::string strValue = std::to_string(static_cast<int>(integral));
  std::string strFractional = std::to_string(static_cast<int>(fractional*10));
  uint8_t emptySpaces = getNumberOfCharsPerRow() - strValue.length() - strFractional.length();
  uint8_t leftFills = static_cast<uint8_t>(emptySpaces / 2.0f);
  resetDots(row_);
  setDot(strValue.length()-1+leftFills, row_);
  strValue.append(strFractional);
  
  setText(strValue, row_, align_);
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setValue(float value_, uint8_t row_, Align align_)
{
  return;
}

//--------------------------------------------------------------------------------------------------

std::string LCDDisplay7Segments::alignText(const std::string& string_, Align align_) const
{
  if(string_.length()>=getNumberOfCharsPerRow())
  {
    return string_.substr(0,getNumberOfCharsPerRow());
  }
  
  std::string strValue(string_);
  switch(align_)
  {
    case Align::Right:
    {
      strValue.insert(0, getNumberOfCharsPerRow()-strValue.length(),' ');
      break;
    }
    case Align::Center:
    {
      uint8_t nFills = getNumberOfCharsPerRow()-strValue.length();
      uint8_t leftFills = static_cast<uint8_t>(nFills / 2.0f);
      strValue.insert(0, leftFills,' ');
      strValue.append(nFills-leftFills,' ');
      break;
    }
    case Align::Left:
    default:
    {
      strValue.append(getNumberOfCharsPerRow()-strValue.length(), ' ');
      break;
    }
  }
  return strValue;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::setDot(uint8_t nDot_, uint8_t row_, bool visible_)
{
  if (row_ > 0 || nDot_ >= getNumberOfCharsPerRow())
  {
    return;
  }
  setDirty(true);
  data()[nDot_] |= 0x01;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplay7Segments::resetDots(uint8_t row_)
{
  if (row_ > 0)
  {
    return;
  }
  setDirty(true);

  for(uint8_t i=0; i < getNumberOfCharsPerRow(); i++)
  {
    data()[i] &= 0xfe;
  }
}

//--------------------------------------------------------------------------------------------------
  
} // kio
} // sl
