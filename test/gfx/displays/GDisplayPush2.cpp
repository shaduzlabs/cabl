/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayPush2.h>

#include "gfx/CanvasTestFunctions.h"
#include "gfx/CanvasTestHelpers.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

namespace
{

std::string pngFileName(const std::string& test_)
{
  return "test-data/gfx/displays/GDisplayPush2-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: constructor", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display;

  CHECK(display.width() == 960);
  CHECK(display.height() == 160);
  CHECK(display.numberOfChunks() == 1);

  CHECK(display.pixel(2000, 2000) == Color());

  display.setPixel(0, 0, {0, 248, 0});
  auto color = display.pixel(0, 0);
  display.setPixel(0, 0, color);
  CHECK(color == display.pixel(0, 0));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: display chunks", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display;
  CHECK(display.dirty());

  display.resetDirtyFlags();
  for (unsigned i = 0; i < display.numberOfChunks(); i++)
  {
    CHECK_FALSE(display.dirtyChunk(i));
  }

  display.lineVertical(0, 0, display.height(), {0xFF});

  for (unsigned i = 0; i < display.numberOfChunks(); i++)
  {
    CHECK(display.dirtyChunk(i));
  }
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: lines", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  lines(&display);
  std::string fileNameSuffix("lines");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: circles", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  circles(&display);
  std::string fileNameSuffix("circles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: triangles", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  triangles(&display);
  std::string fileNameSuffix("triangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: rectangles", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  rectangles(&display);
  std::string fileNameSuffix("rectangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: text", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  text(&display);
  std::string fileNameSuffix("text");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: canvas", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  canvas(&display);
  std::string fileNameSuffix("canvas");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: bitmap", "[gfx][displays][GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  bitmap(&display);
  std::string fileNameSuffix("bitmap");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
