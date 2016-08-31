/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMK1.h>

#include "gfx/CanvasTestHelpers.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: constructor", "[gfx/Canvas/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display;
  CHECK(display.width() == 255);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 22);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("GDisplayMaschineMK1: pixel functions", "[gfx/Canvas/displays/GDisplayMaschineMK1]")
{
  GDisplayMaschineMK1 display;

  SECTION("Gradient")
  {
    for (unsigned j = 0; j < display.width(); j++)
    {
      for (unsigned i = 0; i < display.height(); i++)
      {
        display.setPixel(j, i, {static_cast<uint8_t>(j), 0, 0});
      }
    }

    GDisplayMaschineMK1 displayFromPng;
    REQUIRE(readPng(&displayFromPng, "test-data/gfx/displays/GDisplayMaschineMK1-0.png"));
    CHECK(compare(&display,&displayFromPng));
  }


  SECTION("Misc")
  {
    display.black();
    display.line(0, 0, 2560, 640, {0xff});
    display.rectangleRounded(50, 20, 20, 20, 3, {0xff});
    display.circle(50, 32, 24, {0xff});
    display.rectangleFilled(10, 15, 90, 45, {0xff}, {util::ColorRGB::BlendMode::Invert});
    display.rectangleRoundedFilled(150, 10, 90, 50, 3, {0xff}, {0x55});
    display.text(153,18,"43.",{util::ColorRGB::BlendMode::Invert},"big");
    
    GDisplayMaschineMK1 displayFromPng;
    REQUIRE(readPng(&displayFromPng, "test-data/gfx/displays/GDisplayMaschineMK1-1.png"));
    CHECK(compare(&display,&displayFromPng));
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
