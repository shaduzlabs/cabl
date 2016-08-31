/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMK1.h>

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
  return "test-data/gfx/displays/GDisplayMaschineMK1-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: constructor", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display;
  CHECK(display.width() == 255);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 22);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: lines", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  lines(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("lines")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: circles", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  circles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("circles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: triangles", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  triangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("triangles")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: rectangles", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  rectangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("rectangles")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: text", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  text(&display);
  REQUIRE(pngWrite(&display, pngFileName("text")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: canvas", "[gfx/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display, displayFromPng;
  canvas(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("canvas")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
