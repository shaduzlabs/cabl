/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "gfx/CanvasBase.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class NullCanvas : public CanvasBase<0, 0, 0>
{
public:
  bool dirty() const override
  {
    return false;
  }

  bool dirtyChunk(uint8_t) const override
  {
    return false;
  }

  void resetDirtyFlags() const override
  {
  }

  void setDirtyChunk(uint16_t) const override
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

  void fill(uint8_t) override
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

  void triangle(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void triangleFilled(uint16_t,
    uint16_t,
    uint16_t,
    uint16_t,
    uint16_t,
    uint16_t,
    const util::ColorRGB&,
    const util::ColorRGB&) override
  {
  }

  void rectangle(uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void rectangleFilled(
    uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void rectangleRounded(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&) override
  {
  }

  void rectangleRoundedFilled(
    uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&)
    override
  {
  }

  void circle(uint16_t, uint16_t, uint16_t, const util::ColorRGB&, CircleType) override
  {
  }

  void circleFilled(
    uint16_t, uint16_t, uint16_t, const util::ColorRGB&, const util::ColorRGB&, CircleType) override
  {
  }

  void putBitmap(
    uint16_t, uint16_t, uint16_t, uint16_t, const uint8_t*, const util::ColorRGB&) override
  {
  }

  void putCanvas(const Canvas&, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t) override
  {
  }

  void putCharacter(uint16_t, uint16_t, char, const util::ColorRGB&, const std::string&) override
  {
  }

  void putText(
    uint16_t, uint16_t, const char*, const util::ColorRGB&, const std::string&, uint8_t) override
  {
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
