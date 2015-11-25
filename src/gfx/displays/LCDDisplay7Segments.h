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
namespace cabl
{
class LCDDisplay7Segments : public LCDDisplay
{
  
public:

  LCDDisplay7Segments(uint8_t);
  
  ~LCDDisplay7Segments();
  
  void clear() override;

  bool isDirtyRow(uint8_t row_) const override;
  
  void setCharacter(uint8_t col_, uint8_t row_, char c_) override;

  void setText(const std::string& string_, uint8_t row_, Align align_) override;

  void setText(int value_, uint8_t row_, Align align_) override;

  void setText(double value_, uint8_t row_, Align align_) override;

  void setValue(float value_, uint8_t row_, Align align_) override;
  
private:
  
  std::string alignText(const std::string&, Align align_) const;
  
  void setDot(uint8_t nDot_, uint8_t row_, bool visible_ = true);

  void resetDots(uint8_t row_);
};
  
//--------------------------------------------------------------------------------------------------
  
} // cabl
} // sl
