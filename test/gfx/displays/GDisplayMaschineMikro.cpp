/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMikro.h>

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
  return "test-data/gfx/displays/GDisplayMaschineMikro-" + test_ + ".png";
}

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: constructor", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display;
  CHECK(display.width() == 128);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 4);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: lines", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  lines(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("lines")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: circles", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  circles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("circles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: triangles", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  triangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("triangles")));
 // CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: rectangles", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  rectangles(&display);
  REQUIRE(pngWrite(&display, pngFileName("rectangles")));
//  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: text", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  text(&display);
  REQUIRE(pngWrite(&display, pngFileName("text")));
//  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMikro: canvas", "[gfx/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display, displayFromPng;
  canvas(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("canvas")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
