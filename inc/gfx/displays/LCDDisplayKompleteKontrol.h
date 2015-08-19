/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <stdint.h>

#include "gfx/LCDDisplay.h"

namespace sl
{
namespace kio
{
class LCDDisplayKompleteKontrol : public LCDDisplay
{
  
public:

  LCDDisplayKompleteKontrol();
  
  ~LCDDisplayKompleteKontrol();
  
  void clear() override;

  bool isDirtyRow(uint8_t row_) const override;
  
  void printChar(uint8_t col_, uint8_t row_, char c_) override;

  void setText(const std::string& string_, uint8_t row_) override; 

  void setText(unsigned value_, uint8_t row_) override;

  void setValue(float value_, uint8_t row_) override;
  
private:
  
  void setDot(uint8_t nDot_, uint8_t row_, bool visible_ = true);

  void resetDots(uint8_t row_);

  static constexpr uint8_t kLCDKK_numRows = 3;
  static constexpr uint8_t kLCDKK_numCols = 8;
  
  bool                     m_dirtyFlags[kLCDKK_numRows];
};
  
//--------------------------------------------------------------------------------------------------
  
} // kio
} // sl
