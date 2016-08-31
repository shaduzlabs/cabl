/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayPush2.h>

#include "gfx/CanvasTestHelpers.h"
#include "gfx/CanvasTestFunctions.h"

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

TEST_CASE("GDisplayPush2: constructor", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display;
  
  CHECK(display.width() == 1024);
  CHECK(display.height() == 160);
  CHECK(display.numberOfChunks() == 1);
  
  display.setPixel(0, 0, {0,248,0});
  auto color = display.pixel(0, 0);
  display.setPixel(0, 0, color);
  CHECK(color == display.pixel(0, 0));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: lines", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  lines(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("lines")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: circles", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  circles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("circles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: triangles", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  triangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("triangles")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: rectangles", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  rectangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("rectangles")));
//  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: text", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  text(&display);
  REQUIRE(pngWrite(&display, pngFileName("text")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayPush2: canvas", "[gfx/displays/GDisplayPush2]")
{
  GDisplayPush2 display, displayFromPng;
  canvas(&display);
  
  REQUIRE(pngRead(&displayFromPng, pngFileName("canvas")));
 // CHECK(compare(&display,&displayFromPng));

}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
