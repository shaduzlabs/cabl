/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/GDisplayMaschineMK2.h>

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

TEST_CASE("GDisplayMaschineMK2: constructor", "[gfx/Canvas/displays/GDisplayMaschineMK2]")
{
  GDisplayMaschineMK2 display;
  CHECK(display.width() == 256);
  CHECK(display.height() == 64);
  CHECK(display.numberOfChunks() == 8);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
