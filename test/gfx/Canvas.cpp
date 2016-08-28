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

namespace{
bool matchColorForAllPixels(const Canvas& canvas_, Canvas::Color color_)
{
  for(unsigned x = 0U; x < canvas_.width(); x++)
  {
    for(unsigned y = 0U; y < canvas_.height(); y++)
    {
      if(color_ != canvas_.pixel(x, y))
      {
        return false;
      }
    }
  }
  return true;
}

}

//--------------------------------------------------------------------------------------------------

//  ------------------
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
// | 0000000000000000 |
//  ------------------

TEST_CASE("Canvas constructor", "[gfx/Canvas]")
{
  Canvas c(16, 5, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas convenience functions", "[gfx/Canvas]")
{
  Canvas c(16, 5, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);

  c.black();
  CHECK( matchColorForAllPixels(c, Canvas::Color::Black));

  c.invert();
  CHECK( matchColorForAllPixels(c, Canvas::Color::White));

  c.invert();
  CHECK( matchColorForAllPixels(c, Canvas::Color::Black));

  c.white();
  CHECK( matchColorForAllPixels(c, Canvas::Color::White));
  
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas pixel functions", "[gfx/Canvas]")
{
  Canvas c(16, 5, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);

  c.black();
  CHECK( matchColorForAllPixels(c, Canvas::Color::Black));

  c.setPixel(5,2,Canvas::Color::White);
  CHECK_FALSE( matchColorForAllPixels(c, Canvas::Color::Black));
  c.setPixel(5,2,Canvas::Color::Black);
  CHECK( matchColorForAllPixels(c, Canvas::Color::Black));
}
  
//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
