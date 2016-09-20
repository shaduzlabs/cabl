/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cabl/gfx/Font.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class FontNormal : public Font
{

public:
  inline uint8_t width() const noexcept
  {
    return 5;
  };

  inline uint8_t height() const noexcept
  {
    return 7;
  };

  inline uint8_t charSpacing() const noexcept
  {
    return 1 + width();
  };

  inline uint8_t firstChar() const noexcept
  {
    return 32;
  };

  inline uint8_t lastChar() const noexcept
  {
    return 128;
  };

  inline uint8_t bytesPerLine() const noexcept
  {
    return 1;
  };

  inline bool pixel(uint8_t c_, uint8_t x_, uint8_t y_) const noexcept
  {
    static uint8_t fontData[] = {
#include "data/FONT_normal.h"
    };

    return pixelImpl(fontData, c_, x_, y_);
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
