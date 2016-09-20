/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/gfx/CanvasBase.h"

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

  bool dirtyChunk(unsigned) const override
  {
    return false;
  }

  void resetDirtyFlags() const override
  {
  }

  void setDirtyChunk(unsigned) const override
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

  void line(unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void lineVertical(unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void lineHorizontal(unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void triangle(
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void triangleFilled(unsigned,
    unsigned,
    unsigned,
    unsigned,
    unsigned,
    unsigned,
    const util::ColorRGB&,
    const util::ColorRGB&) override
  {
  }

  void rectangle(unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void rectangleFilled(
    unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&, const util::ColorRGB&) override
  {
  }

  void rectangleRounded(
    unsigned, unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&) override
  {
  }

  void rectangleRoundedFilled(
    unsigned, unsigned, unsigned, unsigned, unsigned, const util::ColorRGB&, const util::ColorRGB&)
    override
  {
  }

  void circle(unsigned, unsigned, unsigned, const util::ColorRGB&, CircleType) override
  {
  }

  void circleFilled(
    unsigned, unsigned, unsigned, const util::ColorRGB&, const util::ColorRGB&, CircleType) override
  {
  }

  void putBitmap(
    unsigned, unsigned, unsigned, unsigned, const uint8_t*, const util::ColorRGB&) override
  {
  }

  void putCanvas(const Canvas&, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned) override
  {
  }

  void putCharacter(unsigned, unsigned, char, const util::ColorRGB&, const std::string&) override
  {
  }

  void putText(
    unsigned, unsigned, const char*, const util::ColorRGB&, const std::string&, unsigned) override
  {
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
