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
  GDisplayDummy() : GDisplay(0, 0, 0)
  {
  }

  void setPixel(uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  util::ColorRGB pixel(uint16_t, uint16_t) const override
  {
    return {0,0,0,0};
  }

  bool isDirty() const override
  {
    return false;
  }

  bool isChunkDirty(uint8_t) const override
  {
    return false;
  }

  void resetDirtyFlags() const override
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

  void drawLine(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawLineVertical(uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawLineHorizontal(uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawTriangle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawFilledTriangle(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void drawRect(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawFilledRect(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void drawRectRounded(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void drawFilledRectRounded(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void drawCircle(uint16_t, uint16_t, uint16_t, const util::ColorRGB&, CircleType) override
  {
  }

  void drawFilledCircle(uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&, CircleType) override
  {
  }

  void drawBitmap(uint16_t, uint16_t, uint16_t, uint16_t, const uint8_t*, const util::ColorRGB&) override
  {
  }

  void draw(const Canvas&, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t) override
  {
  }

  void printChar(uint16_t, uint16_t, char, const util::ColorRGB&, const std::string&) override
  {
  }

  void printStr(uint16_t, uint16_t, const char*, const util::ColorRGB&, const std::string&, uint8_t) override
  {
  }

  const uint8_t* ptr(uint16_t) const override
  {
    return nullptr;
  }

protected:
  //! Initialize the display
  void initializeImpl() override
  {
  }

  //! \return The width of the display in bytes
  uint16_t canvasWidthInBytesImpl() const override
  {
    return 0U;
  }
  
  void setPixelImpl(uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_ = true) override
  {
  }
  
  util::ColorRGB pixelImpl(uint16_t x_, uint16_t y_) const override
  {
    return {0,0,0,0};
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
