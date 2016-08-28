/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>

#include "gfx/LCDDisplay.h"

namespace sl
{
namespace cabl
{
class LCDDisplayGeneric : public LCDDisplay
{

public:
  LCDDisplayGeneric();

  ~LCDDisplayGeneric() = default;

  void clear() override;

  bool isDirtyRow(uint8_t row_) const override;

  void setCharacter(uint8_t col_, uint8_t row_, char c_) override;

  void setText(const std::string& string_, uint8_t row_, Align align_) override;

  void setText(int value_, uint8_t row_, Align align_) override;

  void setText(double value_, uint8_t row_, Align align_) override;

  void setValue(float value_, uint8_t row_, Align align_) override;

private:
  std::string alignText(const std::string&, Align align_) const;

  static constexpr uint8_t kLCDKK_numRows = 4;
  static constexpr uint8_t kLCDKK_numCols = 17;

  bool m_dirtyFlags[kLCDKK_numRows];
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
