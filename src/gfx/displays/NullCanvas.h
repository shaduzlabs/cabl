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

  void line(unsigned, unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void lineVertical(unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void lineHorizontal(unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void triangle(
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void triangleFilled(unsigned,
    unsigned,
    unsigned,
    unsigned,
    unsigned,
    unsigned,
    const Color&,
    const Color&) override
  {
  }

  void rectangle(unsigned, unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void rectangleFilled(
    unsigned, unsigned, unsigned, unsigned, const Color&, const Color&) override
  {
  }

  void rectangleRounded(
    unsigned, unsigned, unsigned, unsigned, unsigned, const Color&) override
  {
  }

  void rectangleRoundedFilled(
    unsigned, unsigned, unsigned, unsigned, unsigned, const Color&, const Color&)
    override
  {
  }

  void circle(unsigned, unsigned, unsigned, const Color&, CircleType) override
  {
  }

  void circleFilled(
    unsigned, unsigned, unsigned, const Color&, const Color&, CircleType) override
  {
  }

  void putBitmap(
    unsigned, unsigned, unsigned, unsigned, const uint8_t*, const Color&) override
  {
  }

  void putCanvas(const Canvas&, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned) override
  {
  }

  void putCharacter(unsigned, unsigned, char, const Color&, const std::string&) override
  {
  }

  void putText(
    unsigned, unsigned, const char*, const Color&, const std::string&, unsigned) override
  {
  }
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
