/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/DynamicCanvas.h>
#include <gfx/displays/GDisplayDummy.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayDummy: constructor", "[gfx][displays][GDisplayDummy]")
{
  GDisplayDummy display;
  CHECK(display.width() == 0);
  CHECK(display.height() == 0);
  CHECK(display.numberOfChunks() == 1);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayDummy: display chunks", "[gfx][displays][GDisplayDummy]")
{
  GDisplayDummy display;

  for (unsigned i = 0; i < display.numberOfChunks(); i++)
  {
    CHECK_FALSE(display.dirtyChunk(i));
  }

  display.lineVertical(0, 0, display.height(), {0xFF});

  for (unsigned i = 0; i < display.numberOfChunks(); i++)
  {
    CHECK_FALSE(display.dirtyChunk(i));
  }
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayDummy: interface", "[gfx][displays][GDisplayDummy]")
{
  GDisplayDummy display;

  CHECK_FALSE(display.dirty());

  display.setPixel(0, 0, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));
  CHECK_FALSE(display.dirtyChunk(10000));

  display.resetDirtyFlags();
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));
  CHECK_FALSE(display.dirtyChunk(10000));

  display.setDirtyChunk(0);
  display.setDirtyChunk(10000);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));
  CHECK_FALSE(display.dirtyChunk(10000));

  display.white();
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.black();
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.black();
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.fill(0x0ff);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.line(0, 0, 100, 100, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.lineVertical(1, 2, 30, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.lineHorizontal(3, 4, 50, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.triangle(10, 20, 30, 40, 50, 60, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.triangleFilled(10, 20, 30, 12, 22, 32, {0xff}, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.rectangle(1, 10, 4, 40, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.rectangleFilled(1, 4, 5, 15, {0xff}, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.rectangleRounded(5, 7, 10, 10, 3, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.rectangleRoundedFilled(8, 2, 100, 200, 5, {0xff}, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.circle(55, 66, 111, {0xff}, Canvas::CircleType::Full);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.circleFilled(11, 55, 66, {0xff}, {0xff}, Canvas::CircleType::Full);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  const uint8_t myBitmap[] = {1, 12, 14, 16, 4, 7, 2, 7, 4, 7, 4, 43};
  display.bitmap(1, 2, 3, 4, myBitmap, {0xff});
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  DynamicCanvas c(10, 10);
  display.canvas(c, 0, 0, 1, 2, 20, 20);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.character(1, 4, 'h', {0xff}, "normal");
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));

  display.text(3, 1, "oh my god it's really late!", {0xff}, "big", 22);
  CHECK_FALSE(display.dirty());
  CHECK_FALSE(display.dirtyChunk(0));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
