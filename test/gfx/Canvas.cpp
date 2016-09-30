/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <lodepng.h>

#include <cabl/gfx/CanvasBase.h>
#include <cabl/gfx/DynamicCanvas.h>
#include <iostream>

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
  return "test-data/gfx/Canvas-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: constructor", "[gfx][Canvas]")
{
  CanvasBase<16, 5> c;
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
  CHECK(c.dirtyChunk(0));
  c.resetDirtyFlags();
  CHECK_FALSE(c.dirtyChunk(0));
  c.setDirtyChunk(0);
  CHECK(c.dirtyChunk(0));

  CHECK(c.pixel(2000, 2000) == Color());

  CanvasBase<16, 5, 80, 0> c2;
  CHECK_FALSE(c2.dirtyChunk(0));
  c2.setDirtyChunk(0);
  CHECK_FALSE(c2.dirtyChunk(0));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: lines", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  lines(&display);
  std::string fileNameSuffix("lines");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: circles", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  circles(&display);
  std::string fileNameSuffix("circles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: triangles", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  triangles(&display);
  std::string fileNameSuffix("triangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: rectangles", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  rectangles(&display);
  std::string fileNameSuffix("rectangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: text", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  text(&display);
  std::string fileNameSuffix("text");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: canvas", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
  canvas(&display);
  std::string fileNameSuffix("canvas");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: bitmap", "[gfx][Canvas]")
{
  CanvasBase<128, 128> display;
  CanvasBase<128, 128> displayFromPng;
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
