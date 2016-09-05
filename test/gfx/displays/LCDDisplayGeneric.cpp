/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/LCDDisplayGeneric.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("LCDDisplayGeneric: constructor", "[gfx][displays][LCDDisplayGeneric]")
{
  LCDDisplayGeneric<8,4> display;
  CHECK(display.width() == 8);
  CHECK(display.height() == 4);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
