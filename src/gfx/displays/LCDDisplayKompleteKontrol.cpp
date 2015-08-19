/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "gfx/displays/LCDDisplayKompleteKontrol.h"

#include <cmath>
#include <stdint.h>

#include "util/Log.h"

namespace
{
  static const uint16_t kLCDDisplayKK_FontData[] =
  {
#include "../fonts/data/FONT_16-seg.h"
  };
}
    
namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------
 
LCDDisplayKompleteKontrol::LCDDisplayKompleteKontrol()
  : LCDDisplay(kLCDKK_numCols,kLCDKK_numRows)
{
  data().resize(48);
  for(uint8_t i = 0; i < kLCDKK_numRows; i++)
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
  if(row_ >= kLCDKK_numRows)
  {
    return false;
  }
  return m_dirtyFlags[row_];
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::printChar(uint8_t col_, uint8_t row_, char c_)
{
  if(row_<1 || row_>=kLCDKK_numRows || col_ >= kLCDKK_numCols)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = (row_ * 16) +col_;
  data()[index++] = kLCDDisplayKK_FontData[static_cast<uint8_t>(c_)] & 0xff;
  data()[index++] = (kLCDDisplayKK_FontData[static_cast<uint8_t>(c_)] >> 8) & 0xff;
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(const std::string& string_, uint8_t row_)
{
  if(row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;
  unsigned index = row_ * 16;

  if(row_==0)
  {
    data()[index++] = 0x07; // 1st bar + surrounding block (3rd bit)
    data()[index++] = 0x03; // Dots (1)
    data()[index++] = 0x03; // 2nd bar
    data()[index++] = 0x03; // Dots (2)
    data()[index++] = 0x03; // 3rd bar
    data()[index++] = 0x03; // Dots (3)
    data()[index++] = 0x03; // 4th bar
    data()[index++] = 0x03; // Dots (4)
    data()[index++] = 0x03; // 5th bar
    data()[index++] = 0x03; // Dots (5)
    data()[index++] = 0x03; // 6th bar
    data()[index++] = 0x03; // Dots (6)
    data()[index++] = 0x03; // 7th bar
    data()[index++] = 0x03; // Dots (7)
    data()[index++] = 0x03; // 8th bar
    data()[index++] = 0x03; // 9th bar
  }
  else
  {
    for(uint8_t i = 0; i<std::min<uint8_t>(string_.length(),8);i++)
    {
      const uint8_t& character = string_.at(i);
      data()[index++] = kLCDDisplayKK_FontData[character] & 0xff;
      data()[index++] = (kLCDDisplayKK_FontData[character] >> 8) & 0xff;
    }
  }
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setText(unsigned value_, uint8_t row_)
{
  
}

//--------------------------------------------------------------------------------------------------

void LCDDisplayKompleteKontrol::setValue(float value_, uint8_t row_)
{
  if(row_ >= kLCDKK_numRows)
  {
    return;
  }
  setDirty(true);
  m_dirtyFlags[row_] = true;

  unsigned index = row_ * 16;
  float val = std::min<float>(value_,1.0f);
  
  if(row_==0)
  {
    uint8_t valInterval = static_cast<uint8_t>(std::round(val*9.0));
    data()[index++] = 0x04 | (valInterval>0? 0x03 : 0x00); // 1st bar + surrounding block (3rd bit)
    data()[index++] = 0x00; // Dots (1)
    data()[index++] = valInterval>1? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (2)
    data()[index++] = valInterval>2? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (3)
    data()[index++] = valInterval>3? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (4)
    data()[index++] = valInterval>4? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (5)
    data()[index++] = valInterval>5? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (6)
    data()[index++] = valInterval>6? 0x03 : 0x00;
    data()[index++] = 0x00; // Dots (7)
    data()[index++] = valInterval>7? 0x03 : 0x00;
    data()[index++] = valInterval>8? 0x03 : 0x00;
  }
  else
  {
    uint8_t valInterval = static_cast<uint8_t>(std::round(val*8.0));
    for(uint8_t i = 0; i<8;i++)
    {
      if(valInterval>i)
      {
        data()[index++] = kLCDDisplayKK_FontData[43] & 0xff;
        data()[index++] = (kLCDDisplayKK_FontData[43] >> 8) & 0xff;
      }
      else{
        data()[index++] = kLCDDisplayKK_FontData[0];
        data()[index++] = kLCDDisplayKK_FontData[0];
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
  
} // kio
} // sl
