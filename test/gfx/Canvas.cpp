/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <gfx/Canvas.h>
#include <iostream>
namespace sl
{
namespace cabl
{
namespace test
{

namespace
{
bool matchColorForAllPixels(const Canvas& canvas_, Canvas::Color color_)
{
  for (unsigned x = 0U; x < canvas_.width(); x++)
  {
    for (unsigned y = 0U; y < canvas_.height(); y++)
    {
      if (color_ != canvas_.pixel(x, y))
      {
        return false;
      }
    }
  }
  return true;
}
}

//--------------------------------------------------------------------------------------------------

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
  CHECK(matchColorForAllPixels(c, Canvas::Color::Black));

  c.invert();
  CHECK(matchColorForAllPixels(c, Canvas::Color::White));

  c.invert();
  CHECK(matchColorForAllPixels(c, Canvas::Color::Black));

  c.white();
  CHECK(matchColorForAllPixels(c, Canvas::Color::White));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas pixel functions", "[gfx/Canvas]")
{
  Canvas c(16, 5, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);

  c.black();
  CHECK(matchColorForAllPixels(c, Canvas::Color::Black));

  c.setPixel(5, 2, Canvas::Color::White);
  CHECK_FALSE(matchColorForAllPixels(c, Canvas::Color::Black));
  c.setPixel(5, 2, Canvas::Color::Black);
  CHECK(matchColorForAllPixels(c, Canvas::Color::Black));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas drawing: rectangles", "[gfx/Canvas]")
{
  Canvas c(16, 9, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);
  std::string expected;

  c.black();
  c.drawRect(1, 1, 14, 7, Canvas::Color::White);
  expected = "\n░░░░░░░░░░░░░░░░"
             "\n░██████████████░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░██████████████░"
             "\n░░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);

  c.black();
  c.drawFilledRect(1, 1, 14, 7, Canvas::Color::White, Canvas::Color::White);
  expected = "\n░░░░░░░░░░░░░░░░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);

  c.black();
  c.drawFilledRect(1, 1, 14, 7, Canvas::Color::Black, Canvas::Color::White);
  expected = "\n░░░░░░░░░░░░░░░░"
             "\n░░░░░░░░░░░░░░░░"
             "\n░░████████████░░"
             "\n░░████████████░░"
             "\n░░████████████░░"
             "\n░░████████████░░"
             "\n░░████████████░░"
             "\n░░░░░░░░░░░░░░░░"
             "\n░░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);
  
  c.black();
  c.drawFilledRect(1, 1, 14, 7, Canvas::Color::White, Canvas::Color::White);
  c.drawFilledRect(5, 0, 6, 9, Canvas::Color::Invert, Canvas::Color::Invert);
  c.drawFilledRect(0, 3, 16, 3, Canvas::Color::Invert, Canvas::Color::Invert);
  expected = "\n░░░░░██████░░░░░"
             "\n░████░░░░░░████░"
             "\n░████░░░░░░████░"
             "\n█░░░░██████░░░░█"
             "\n█░░░░██████░░░░█"
             "\n█░░░░██████░░░░█"
             "\n░████░░░░░░████░"
             "\n░████░░░░░░████░"
             "\n░░░░░██████░░░░░";
  CHECK(c.string("░", "█") == expected);
  
  c.black();
  c.drawRectRounded(1, 1, 14, 7, 3, Canvas::Color::White);
  expected = "\n░░░░░░░░░░░░░░░░"
             "\n░░░██████████░░░"
             "\n░░█░░░░░░░░░░█░░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░█░░░░░░░░░░░░█░"
             "\n░░█░░░░░░░░░░█░░"
             "\n░░░██████████░░░"
             "\n░░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);
  
    
  c.black();
  c.drawFilledRectRounded( 1, 1, 14, 7, 3, Canvas::Color::White, Canvas::Color::White);
  expected = "\n░░░░░░░░░░░░░░░░"
             "\n░░░██████████░░░"
             "\n░░████████████░░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░██████████████░"
             "\n░░████████████░░"
             "\n░░░██████████░░░"
             "\n░░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);

  c.white();
  c.drawFilledRectRounded( 2, 2, 12, 5, 3, Canvas::Color::Invert, Canvas::Color::Invert);
  expected = "\n████████████████"
             "\n████████████████"
             "\n███░░░░░░░░░░███"
             "\n██░░░░░░░░░░░░██"
             "\n██░░░░░░░░░░░░██"
             "\n██░░░░░░░░░░░░██"
             "\n███░░░░░░░░░░███"
             "\n████████████████"
             "\n████████████████";
  //CHECK(c.string("░", "█") == expected);
  //\todo Fix rendering of filled rounded rectangles
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas drawing: triangles", "[gfx/Canvas]")
{
  Canvas c(16, 9, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);
  std::string expected;

  c.black();
  c.drawTriangle(0, 0, 15, 0, 0, 8, Canvas::Color::White);
  expected = "\n████████████████"
             "\n█░░░░░░░░░░░░██░"
             "\n█░░░░░░░░░░██░░░"
             "\n█░░░░░░░░██░░░░░"
             "\n█░░░░░░██░░░░░░░"
             "\n█░░░░██░░░░░░░░░"
             "\n█░░██░░░░░░░░░░░"
             "\n███░░░░░░░░░░░░░"
             "\n█░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);

  c.black();
  c.drawFilledTriangle(0, 0, 15, 0, 0, 8, Canvas::Color::White, Canvas::Color::White);
  expected = "\n████████████████"
             "\n███████████████░"
             "\n█████████████░░░"
             "\n███████████░░░░░"
             "\n█████████░░░░░░░"
             "\n███████░░░░░░░░░"
             "\n█████░░░░░░░░░░░"
             "\n███░░░░░░░░░░░░░"
             "\n█░░░░░░░░░░░░░░░";
  CHECK(c.string("░", "█") == expected);

  c.black();
  c.drawFilledTriangle(0, 0, 15, 0, 0, 8, Canvas::Color::White, Canvas::Color::White);
  c.drawFilledTriangle(15, 8, 0, 0, 0, 8, Canvas::Color::Invert, Canvas::Color::Invert);
  expected = "\n░███████████████"
             "\n░░█████████████░"
             "\n░░░░█████████░░░"
             "\n░░░░░░█████░░░░░"
             "\n░░░░░░░░█░░░░░░░"
             "\n░░░░░░░███░░░░░░"
             "\n░░░░░███████░░░░"
             "\n░░░███████████░░"
             "\n░███████████████";
  CHECK(c.string("░", "█") == expected);

}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas drawing: circles", "[gfx/Canvas]")
{
  Canvas c(16, 9, Canvas::Allocation::OneBytePacksOneRowOfEightPixels);
  std::string expected;

  c.black();
  c.drawCircle(8, 4, 4, Canvas::Color::White);
  expected = "\n░░░░░░░███░░░░░░"
             "\n░░░░░██░░░██░░░░"
             "\n░░░░░█░░░░░█░░░░"
             "\n░░░░█░░░░░░░█░░░"
             "\n░░░░█░░░░░░░█░░░"
             "\n░░░░█░░░░░░░█░░░"
             "\n░░░░░█░░░░░█░░░░"
             "\n░░░░░██░░░██░░░░"
             "\n░░░░░░░███░░░░░░";
  CHECK(c.string("░", "█") == expected);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
