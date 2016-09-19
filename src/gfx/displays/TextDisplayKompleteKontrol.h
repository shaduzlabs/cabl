/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>

#include "cabl/gfx/TextDisplay.h"

namespace sl
{
namespace cabl
{
class TextDisplayKompleteKontrol : public TextDisplayBase<8, 3, 48>
{

public:
  void putCharacter(unsigned col_, unsigned row_, char c_) override;

  void putText(const std::string& string_, unsigned row_, Alignment align_) override;

  void putText(int value_, unsigned row_, Alignment align_) override;

  void putText(double value_, unsigned row_, Alignment align_) override;

  void putValue(float value_, unsigned row_, Alignment align_) override;

private:
  std::string alignText(const std::string&, Alignment align_) const;

  void setDot(unsigned nDot_, unsigned row_, bool visible_ = true);

  void resetDots(unsigned row_);
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
