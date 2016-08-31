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
  GDisplayDummy() : GDisplay(0U, 0U, 0U)
  {
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
    return 0U;
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

  void line(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void lineVertical(uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void lineHorizontal(uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void triangle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void triangleFilled(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void rectangle(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void rectangleFilled(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void rectangleRounded(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void rectangleRoundedFilled(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void circle(uint16_t, uint16_t, uint16_t, const util::ColorRGB&, CircleType) override
  {
  }

  void circleFilled(uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&, CircleType) override
  {
  }

  void bitmap(uint16_t, uint16_t, uint16_t, uint16_t, const uint8_t*, const util::ColorRGB&) override
  {
  }

  void canvas(const Canvas&, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t) override
  {
  }

  void character(uint16_t, uint16_t, char, const util::ColorRGB&, const std::string&) override
  {
  }

  void text(uint16_t, uint16_t, const char*, const util::ColorRGB&, const std::string&, uint8_t) override
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
    return {};
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
