/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMK1.h>

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
  return "test-data/gfx/displays/GDisplayMaschineMK1-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: constructor", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display;
  CHECK(display.width() == 255);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 22);

  CHECK(display.pixel(2000, 2000) == Color());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: display chunks", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display;
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

TEST_CASE("GDisplayMaschineMK1: lines", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  lines(&display);
  std::string fileNameSuffix("lines");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: circles", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  circles(&display);
  std::string fileNameSuffix("circles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: triangles", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  triangles(&display);
  std::string fileNameSuffix("triangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: rectangles", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  rectangles(&display);
  std::string fileNameSuffix("rectangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: text", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  text(&display);
  std::string fileNameSuffix("text");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: canvas", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  canvas(&display);
  std::string fileNameSuffix("canvas");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: bitmap", "[gfx][displays][GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
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
