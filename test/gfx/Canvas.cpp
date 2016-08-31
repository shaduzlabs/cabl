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


#include <gfx/displays/GDisplayMaschineMK1.h>
#include <gfx/displays/GDisplayMaschineMK2.h>
#include <gfx/displays/GDisplayMaschineMikro.h>
#include <gfx/displays/GDisplayPush2.h>

#include "gfx/CanvasTestHelpers.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

using CTH = CanvasTestHelper;

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: constructor", "[gfx/Canvas]")
{
  Canvas c(16, 5);
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: convenience functions", "[gfx/Canvas]")
{
  Canvas c(16, 5);

  c.black();
  CHECK(matchColorForAllPixels(&c, {0}));

  c.invert();
  CHECK(matchColorForAllPixels(&c, {0xff}));

  c.invert();
  CHECK(matchColorForAllPixels(&c, {0x00}));

  c.white();
  CHECK(matchColorForAllPixels(&c, {0xff}));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: pixel functions", "[gfx/Canvas]")
{
  Canvas c(16, 5);

  c.black();
  CHECK(matchColorForAllPixels(&c, {0x00}));

  c.setPixel(5, 2, {0xff});
  CHECK_FALSE(matchColorForAllPixels(&c, {0x00}));
  c.setPixel(5, 2, {0x00});
  CHECK(matchColorForAllPixels(&c, {0x00}));

  c.setPixel(50, 2, {0x00});
  CHECK(matchColorForAllPixels(&c, {0x00}));

  c.setPixel(5, 20, {0x00});
  CHECK(matchColorForAllPixels(&c, {0x00}));

  c.setPixel(5, 2, {});
  CHECK(matchColorForAllPixels(&c, {0x00}));

  CHECK(c.pixel(100, 100) == util::ColorRGB());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing rectangles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.rectangle(1, 1, 14, 7, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░██████████████░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░██████████████░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.rectangleFilled(1, 1, 14, 7, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.rectangleFilled(1, 1, 14, 7, {0x00}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░░░░░░░░░░░░░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.rectangleFilled(1, 1, 14, 7, {0xff}, {0xff});
  c.rectangleFilled(
    5, 0, 6, 9, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  c.rectangleFilled(
    0, 3, 16, 3, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  CHECK(CTH::displayContent(c) == "\n░░░░░██████░░░░░"
                                  "\n░████░░░░░░████░"
                                  "\n░████░░░░░░████░"
                                  "\n█░░░░██████░░░░█"
                                  "\n█░░░░██████░░░░█"
                                  "\n█░░░░██████░░░░█"
                                  "\n░████░░░░░░████░"
                                  "\n░████░░░░░░████░"
                                  "\n░░░░░██████░░░░░");


  c.black();
  c.rectangleRounded(1, 1, 14, 7, 3, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░██████████░░░"
                                  "\n░░█░░░░░░░░░░█░░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░░█░░░░░░░░░░█░░"
                                  "\n░░░██████████░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.rectangleRoundedFilled(1, 1, 14, 7, 3, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░██████████░░░"
                                  "\n░░████████████░░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░░████████████░░"
                                  "\n░░░██████████░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.white();
  c.rectangleRoundedFilled(
    2, 2, 12, 5, 3, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  /*
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n████████████████"
                                  "\n███░░░░░░░░░░███"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n███░░░░░░░░░░███"
                                  "\n████████████████"
                                  "\n████████████████");
                                  */
  // CHECK(c.string("░", "█") == expected);
  //\todo Fix rendering of filled rounded rectangles
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing triangles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.triangle(0, 0, 15, 0, 0, 8, {0xff});
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n█░░░░░░░░░░░░██░"
                                  "\n█░░░░░░░░░░██░░░"
                                  "\n█░░░░░░░░██░░░░░"
                                  "\n█░░░░░░██░░░░░░░"
                                  "\n█░░░░██░░░░░░░░░"
                                  "\n█░░██░░░░░░░░░░░"
                                  "\n███░░░░░░░░░░░░░"
                                  "\n█░░░░░░░░░░░░░░░");

  c.black();
  c.triangleFilled(0, 0, 15, 0, 0, 8, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n███████████████░"
                                  "\n█████████████░░░"
                                  "\n███████████░░░░░"
                                  "\n█████████░░░░░░░"
                                  "\n███████░░░░░░░░░"
                                  "\n█████░░░░░░░░░░░"
                                  "\n███░░░░░░░░░░░░░"
                                  "\n█░░░░░░░░░░░░░░░");

  c.black();
  c.triangleFilled(0, 0, 15, 0, 0, 8, {0xff}, {0xff});
  c.triangleFilled(
    15, 8, 0, 0, 0, 8, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  CHECK(CTH::displayContent(c) == "\n░███████████████"
                                  "\n░░█████████████░"
                                  "\n░░░░█████████░░░"
                                  "\n░░░░░░█████░░░░░"
                                  "\n░░░░░░░░█░░░░░░░"
                                  "\n░░░░░░░███░░░░░░"
                                  "\n░░░░░███████░░░░"
                                  "\n░░░███████████░░"
                                  "\n░███████████████");
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing circles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.circle(8, 4, 4, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░███░░░░░░"
                                  "\n░░░░░██░░░██░░░░"
                                  "\n░░░░░█░░░░░█░░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░░█░░░░░█░░░░"
                                  "\n░░░░░██░░░██░░░░"
                                  "\n░░░░░░░███░░░░░░");

  c.white();
  c.circleFilled(8, 4, 4, {0x00}, {0x00});
  CHECK(CTH::displayContent(c) == "\n███████░░░██████"
                                  "\n█████░░░░░░░████"
                                  "\n█████░░░░░░░████"
                                  "\n████░░░░░░░░░███"
                                  "\n████░░░░░░░░░███"
                                  "\n████░░░░░░░░░███"
                                  "\n█████░░░░░░░████"
                                  "\n█████░░░░░░░████"
                                  "\n███████░░░██████");
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
