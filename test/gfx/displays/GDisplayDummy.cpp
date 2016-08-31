/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayDummy.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayDummy: constructor", "[gfx/Canvas/displays/GDisplayDummy]")
{
  GDisplayDummy display;
  CHECK(display.width() == 0);
  CHECK(display.height() == 0);
  CHECK(display.numberOfChunks() == 0);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayDummy: interface", "[gfx/Canvas/displays/GDisplayDummy]")
{
  GDisplayDummy display;

  CHECK_FALSE(display.isDirty());

  display.setPixel(0,0,{0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));
  CHECK_FALSE(display.isChunkDirty(10000));

  display.resetDirtyFlags();
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));
  CHECK_FALSE(display.isChunkDirty(10000));
  
  display.setDirtyChunks(0, 0);
  display.setDirtyChunks(10000, 10000);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));
  CHECK_FALSE(display.isChunkDirty(10000));
  
  display.white();
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.black();
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.black();
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.fillPattern(0x0ff);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.line(0, 0, 100, 100, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.lineVertical(1, 2, 30, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.lineHorizontal(3, 4, 50, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.triangle(10, 20, 30, 40, 50, 60, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.triangleFilled(
    10, 20, 30, 12, 22, 32, {0xff}, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.rectangle(1, 10, 4, 40, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.rectangleFilled(1, 4, 5, 15, {0xff}, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.rectangleRounded(5, 7, 10, 10, 3, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.rectangleRoundedFilled(
    8, 2, 100, 200, 5, {0xff}, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.circle(55, 66, 111, {0xff}, Canvas::CircleType::Full);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.circleFilled(11, 55, 66, {0xff}, {0xff}, Canvas::CircleType::Full);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  const uint8_t myBitmap[] = {1,12,14,16,4,7,2,7,4,7,4,43};
  display.bitmap(1, 2, 3, 4, myBitmap, {0xff});
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.canvas({10,10}, 0, 0, 1, 2, 20, 20);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.character(1, 4, 'h', {0xff}, "normal");
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  display.text(3, 1, "oh my god it's really late!", {0xff}, "big", 22);
  CHECK_FALSE(display.isDirty());
  CHECK_FALSE(display.isChunkDirty(0));

  CHECK(display.ptr(0) == nullptr);
  CHECK(display.ptr(10000) == nullptr);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
