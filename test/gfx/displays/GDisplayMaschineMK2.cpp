/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMK2.h>

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
  return "test-data/gfx/displays/GDisplayMaschineMK2-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: constructor", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display;
  CHECK(display.width() == 256);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 8);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: lines", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  lines(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("lines")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: circles", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  circles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("circles")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: triangles", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  triangles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("triangles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: rectangles", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  rectangles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("rectangles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: text", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  text(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("text")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK2: canvas", "[gfx/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display, displayFromPng;
  canvas(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("canvas")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
