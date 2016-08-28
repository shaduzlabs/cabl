/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <gfx/Canvas.h>

namespace sl
{
namespace cabl
{
namespace test
{


//--------------------------------------------------------------------------------------------------

//  ------------------
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
//  ------------------

TEST_CASE("Constructors, reset, bool operator", "[gfx/Canvas]")
{
  Canvas c(16, 5, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
}


//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
