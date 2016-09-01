/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <lodepng.h>

#include <gfx/Canvas.h>
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

TEST_CASE("Canvas: constructor", "[gfx/Canvas]")
{
  Canvas c(16, 5);
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
  
  CHECK( c.pixel(2000,2000) == util::ColorRGB());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: lines", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  lines(&display);
  std::string fileNameSuffix("lines");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: circles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  circles(&display);
  std::string fileNameSuffix("circles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: triangles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  triangles(&display);
  std::string fileNameSuffix("triangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: rectangles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  rectangles(&display);
  std::string fileNameSuffix("rectangles");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: text", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  text(&display);
  std::string fileNameSuffix("text");
#ifdef DO_WRITE_PICTURES
  REQUIRE(pngWrite(&display, pngFileName(fileNameSuffix)));
#endif
  REQUIRE(pngRead(&displayFromPng, pngFileName(fileNameSuffix)));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: canvas", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  canvas(&display);
  std::string fileNameSuffix("canvas");
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
