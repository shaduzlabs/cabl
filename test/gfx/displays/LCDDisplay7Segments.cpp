/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/LCDDisplay7Segments.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("LCDDisplay7Segments: constructor", "[gfx][displays][LCDDisplay7Segments]")
{
  LCDDisplay7Segments<19> display;
  CHECK(display.width() == 19);
  CHECK(display.height() == 1);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
