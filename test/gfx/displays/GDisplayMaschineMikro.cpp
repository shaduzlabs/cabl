/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMikro.h>

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

TEST_CASE("GDisplayMaschineMikro: constructor", "[gfx/Canvas/displays/GDisplayMaschineMikro]")
{
  GDisplayMaschineMikro display;
  CHECK(display.width() == 128);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 4);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
