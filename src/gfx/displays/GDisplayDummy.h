/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "gfx/GDisplay.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class GDisplayDummy : public GDisplay
{
public:
  GDisplayDummy() : GDisplay(0, 0, 0, Canvas::Allocation::None)
  {
  }

  void setPixel(uint16_t, uint16_t, Color) override
  {
  }

  Color pixel(uint16_t, uint16_t) const override
  {
    return Color::None;
  }

  bool isDirty() const override
  {
    return false;
  }

  bool isChunkDirty(uint8_t) const override
  {
    return false;
  }

  void resetDirtyFlags() override
  {
  }

  virtual uint8_t numberOfChunks() const noexcept override
  {
    return 0;
  }

  void setDirtyChunks(uint16_t, uint16_t) override
  {
  }

  void white() override
  {
  }

  void black() override
  {
  }

  void invert() override
  {
  }

  void fillPattern(uint8_t) override
  {
  }

  void drawLine(uint16_t, uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawLineVertical(uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawLineHorizontal(uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawTriangle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawFilledTriangle(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, Color, Color) override
  {
  }

  void drawRect(uint16_t, uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawFilledRect(uint16_t, uint16_t, uint16_t, uint16_t, Color, Color) override
  {
  }

  void drawRectRounded(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, Color) override
  {
  }

  void drawFilledRectRounded(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, Color, Color) override
  {
  }

  void drawCircle(uint16_t, uint16_t, uint16_t, Color, CircleType) override
  {
  }

  void drawFilledCircle(uint16_t, uint16_t, uint16_t, Color, Color, CircleType) override
  {
  }

  void drawBitmap(uint16_t, uint16_t, uint16_t, uint16_t, const uint8_t*, Color) override
  {
  }

  void draw(const Canvas&, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t) override
  {
  }

  void printChar(uint16_t, uint16_t, char, Font*, Color) override
  {
  }

  void printStr(uint16_t, uint16_t, const char*, FontType, Color, uint8_t) override
  {
  }

  void printStr(uint16_t, uint16_t, const char*, Font*, Color, uint8_t) override
  {
  }

  void setDefaultFont(FontType) override
  {
  }

  const uint8_t* ptr(uint16_t) const override
  {
    return nullptr;
  }

protected:
  void setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_ = true) override
  {
  }

  Color pixelImpl(uint16_t x_, uint16_t y_) const override
  {
    return Color::None;
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
