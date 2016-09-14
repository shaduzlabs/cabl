/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/util/Macros.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/**
  \class Font
  \brief The font base class

*/
class Font
{

public:
  virtual uint8_t width() const noexcept = 0;
  virtual uint8_t height() const noexcept = 0;
  virtual uint8_t charSpacing() const noexcept = 0;

  virtual uint8_t firstChar() const noexcept = 0;
  virtual uint8_t lastChar() const noexcept = 0;

  virtual uint8_t bytesPerLine() const noexcept = 0;

  virtual bool pixel(uint8_t char_, uint8_t x_, uint8_t y_) const noexcept = 0;

  virtual inline bool pixelImpl(uint8_t* pFontData_, uint8_t c_, uint8_t x_, uint8_t y_) const
    noexcept
  {
    if (c_ > lastChar() || x_ >= width() || y_ >= height())
    {
      return false;
    }

    if (bytesPerLine() == 1)
    {
      return ((pFontData_[(c_ * height()) + y_] & (0x080 >> x_)) > 0);
    }
    else
    {
      return (
        (pFontData_[(c_ * height()) + (y_ * bytesPerLine()) + (x_ >> 3)] & (0x080 >> (x_ % 8)))
        > 0);
    }
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
