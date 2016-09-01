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
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: lines", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  lines(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("lines")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: circles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  circles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("circles")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: triangles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  triangles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("triangles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: rectangles", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  rectangles(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("rectangles")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: text", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  text(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("text")));
  CHECK(compare(&display,&displayFromPng));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: canvas", "[gfx/Canvas]")
{
  Canvas display(128, 128), displayFromPng(128, 128);
  canvas(&display);
  REQUIRE(pngRead(&displayFromPng, pngFileName("canvas")));
  CHECK(compare(&display, &displayFromPng));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
