/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/LCDDisplayKompleteKontrol.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("LCDDisplayKompleteKontrol: constructor", "[gfx][displays][LCDDisplayKompleteKontrol]")
{
  LCDDisplayKompleteKontrol display;
  CHECK(display.width() == 8);
  CHECK(display.height() == 3);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
