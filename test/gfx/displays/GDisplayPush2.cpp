/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
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

TEST_CASE("GDisplayPush2: constructor", "[gfx/Canvas/displays/GDisplayPush2]")
{
  GDisplayPush2 display;
  CHECK(display.width() == 1024);
  CHECK(display.height() == 160);
  CHECK(display.numberOfChunks() == 1);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
