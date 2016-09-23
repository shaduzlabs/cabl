/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMikro.h>

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
  return "test-data/gfx/displays/GDisplayMaschineMikro-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: constructor", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display;
  CHECK(display.width() == 128);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 4);

  CHECK(display.pixel(2000, 2000) == Color());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: display chunks", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display;
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

TEST_CASE("GDisplayMaschineMikro: lines", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  lines(&display);
  std::string fileNameSuffix("lines");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: circles", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  circles(&display);
  std::string fileNameSuffix("circles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: triangles", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  triangles(&display);
  std::string fileNameSuffix("triangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: rectangles", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  rectangles(&display);
  std::string fileNameSuffix("rectangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: text", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  text(&display);
  std::string fileNameSuffix("text");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: canvas", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  canvas(&display);
  std::string fileNameSuffix("canvas");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: bitmap", "[gfx][displays][GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
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
