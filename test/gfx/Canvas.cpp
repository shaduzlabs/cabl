/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <lodepng.h>

#include <gfx/Canvas.h>
#include <iostream>


#include <gfx/displays/GDisplayMaschineMK1.h>
#include <gfx/displays/GDisplayMaschineMK2.h>
#include <gfx/displays/GDisplayMaschineMikro.h>
#include <gfx/displays/GDisplayPush2.h>

#include "gfx/CanvasTestHelper.h"

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

namespace
{

//--------------------------------------------------------------------------------------------------

bool matchColorForAllPixels(const Canvas& canvas_, const util::ColorRGB& color_)
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

//--------------------------------------------------------------------------------------------------

bool compare(const Canvas* c1_, const Canvas* c2_)
{
  if (c1_->width() != c2_->width() || c1_->height() != c2_->height())
  {
    return false;
  }
  for (unsigned x = 0U; x < c1_->width(); x++)
  {
    for (unsigned y = 0U; y < c1_->height(); y++)
    {
      if (c1_->pixel(x, y) != c2_->pixel(x, y))
      {
        return false;
      }
    }
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

void writePng(const Canvas* c_, const std::string& fileName_)
{
  std::vector<uint8_t> pixels(c_->width() * c_->height() * 4);
  for (unsigned y = 0; y < c_->height(); y++)
  {
    for (unsigned x = 0; x < c_->width(); x++)
    {
      unsigned index = 4 * ((y * c_->width()) + x);
      util::ColorRGB color = c_->pixel(x, y);
      pixels[index++] = color.red();
      pixels[index++] = color.green();
      pixels[index++] = color.blue();
      pixels[index++] = 0xff;
    }
  }

  std::vector<unsigned char> png;
  unsigned error = lodepng::encode(png, pixels, c_->width(), c_->height());
  if (!error)
  {
    lodepng::save_file(png, fileName_.c_str());
  }
  else
  {
    std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  }
}

//--------------------------------------------------------------------------------------------------

Canvas readPng(const std::string& fileName_)
{
  std::vector<unsigned char> png;
  std::vector<unsigned char> pixels;
  unsigned width, height;

  // load and decode
  unsigned error = lodepng::load_file(png, fileName_.c_str());
  if (!error)
  {
    error = lodepng::decode(pixels, width, height, png);
  }

  if (error)
  {
    std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return Canvas(0, 0);
  }
  Canvas c(width, height);

  for (unsigned y = 0; y < height; y++)
  {
    for (unsigned x = 0; x < width; x++)
    {
      unsigned index = 4 * ((y * width) + x);
      c.setPixel(x, y, {pixels[index], pixels[index + 1], pixels[index + 2]});
    }
  }
  return c;
}

//--------------------------------------------------------------------------------------------------

} // namespace

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: constructor", "[gfx/Canvas]")
{
  Canvas c(16, 5);
  CHECK(c.width() == 16);
  CHECK(c.height() == 5);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: convenience functions", "[gfx/Canvas]")
{
  Canvas c(16, 5);

  c.black();
  CHECK(matchColorForAllPixels(c, {0}));

  c.invert();
  CHECK(matchColorForAllPixels(c, {0xff}));

  c.invert();
  CHECK(matchColorForAllPixels(c, {0x00}));

  c.white();
  CHECK(matchColorForAllPixels(c, {0xff}));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: pixel functions", "[gfx/Canvas]")
{
  Canvas c(16, 5);

  c.black();
  CHECK(matchColorForAllPixels(c, {0x00}));

  c.setPixel(5, 2, {0xff});
  CHECK_FALSE(matchColorForAllPixels(c, {0x00}));
  c.setPixel(5, 2, {0x00});
  CHECK(matchColorForAllPixels(c, {0x00}));

  c.setPixel(50, 2, {0x00});
  CHECK(matchColorForAllPixels(c, {0x00}));

  c.setPixel(5, 20, {0x00});
  CHECK(matchColorForAllPixels(c, {0x00}));

  c.setPixel(5, 2, {});
  CHECK(matchColorForAllPixels(c, {0x00}));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: allocation types and displays", "[gfx/Canvas]")
{
  std::string expected;

  GDisplayMaschineMikro c_0;
  c_0.black();
  c_0.drawLine(0, 0, 1280, 640, {0xff});
  c_0.drawRectRounded(50, 20, 20, 20, 3, {0xff});
  c_0.drawCircle(50, 32, 24, {0xff});
  c_0.drawFilledRect(10, 15, 90, 45, {0xff}, {util::ColorRGB::BlendMode::Invert});
  for (unsigned i = 0; i < 1024; i++)
  {
    c_0.setPixel(i, 31, {0, 0, 0xff});
    c_0.setPixel(i, 32, {0, 0, 0xff});
  }
  writePng(&c_0, "test-data/gfx/GDisplayMaschineMikro-0.png");

  GDisplayMaschineMK1 c_1;
  c_1.black();
  c_1.drawLine(0, 0, 2560, 640, {0xff});
  c_1.drawRectRounded(50, 20, 20, 20, 3, {0xff});
  c_1.drawCircle(50, 32, 24, {0xff});
  c_1.drawFilledRect(10, 15, 90, 45, {0xff}, {util::ColorRGB::BlendMode::Invert});
  c_1.printStr(10, 18, "Hey ho!", {util::ColorRGB::BlendMode::Invert});
  for (unsigned j = 0; j < 256; j++)
  {
    for (unsigned i = 48; i < 64; i++)
    {
      c_1.setPixel(j, i, {static_cast<uint8_t>(j), 0, 0});
    }
  }
  writePng(&c_1, "test-data/gfx/GDisplayMaschineMK1-0.png");

  GDisplayMaschineMK2 c_2;
  c_2.black();
  c_2.drawLine(0, 0, 2560, 640, {0xff});
  c_2.drawRectRounded(50, 20, 20, 20, 3, {0xff});
  c_2.drawCircle(50, 32, 24, {0xff});
  c_2.drawFilledRect(10, 15, 90, 45, {0xff}, {util::ColorRGB::BlendMode::Invert});
  for (unsigned i = 0; i < 1024; i++)
  {
    c_2.setPixel(i, 31, {0, 0, 0xff});
    c_2.setPixel(i, 32, {0, 0, 0xff});
  }
  writePng(&c_2, "test-data/gfx/GDisplayMaschineMK2-0.png");

  GDisplayPush2 c_3;
  c_3.black();
  c_3.drawLine(0, 0, 2560, 640, {0xff});
  c_3.drawRectRounded(50, 20, 20, 20, 3, {0xff});
  c_3.drawCircle(50, 32, 24, {0xff});
  c_3.drawFilledRect(10, 15, 90, 45, {0xff}, {0xff,0,0});
  c_3.drawFilledRect(43, 26, 800, 125, {0xff}, {util::ColorRGB::BlendMode::Invert});
  for (unsigned i = 0; i < 1024; i++)
  {
    c_3.setPixel(i, 79, {0, 0xff, 0xff});
    c_3.setPixel(i, 80, {0, 0, 0xff});
  }
  c_3.printStr(10, 18, "Hey ho!", {util::ColorRGB::BlendMode::Invert},"big",4);

  writePng(&c_3, "test-data/gfx/GDisplayPush2-0.png");
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing rectangles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.drawRect(1, 1, 14, 7, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░██████████████░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░██████████████░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledRect(1, 1, 14, 7, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledRect(1, 1, 14, 7, {0x00}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░░░░░░░░░░░░░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░████████████░░"
                                  "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledRect(1, 1, 14, 7, {0xff}, {0xff});
  c.drawFilledRect(5, 0, 6, 9, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  c.drawFilledRect(0, 3, 16, 3, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  CHECK(CTH::displayContent(c) == "\n░░░░░██████░░░░░"
                                  "\n░████░░░░░░████░"
                                  "\n░████░░░░░░████░"
                                  "\n█░░░░██████░░░░█"
                                  "\n█░░░░██████░░░░█"
                                  "\n█░░░░██████░░░░█"
                                  "\n░████░░░░░░████░"
                                  "\n░████░░░░░░████░"
                                  "\n░░░░░██████░░░░░");


  c.black();
  c.drawRectRounded(1, 1, 14, 7, 3, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░██████████░░░"
                                  "\n░░█░░░░░░░░░░█░░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░█░░░░░░░░░░░░█░"
                                  "\n░░█░░░░░░░░░░█░░"
                                  "\n░░░██████████░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledRectRounded(1, 1, 14, 7, 3, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░░░░░░░░░░"
                                  "\n░░░██████████░░░"
                                  "\n░░████████████░░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░██████████████░"
                                  "\n░░████████████░░"
                                  "\n░░░██████████░░░"
                                  "\n░░░░░░░░░░░░░░░░");

  c.white();
  c.drawFilledRectRounded(2, 2, 12, 5, 3, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  /*
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n████████████████"
                                  "\n███░░░░░░░░░░███"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n██░░░░░░░░░░░░██"
                                  "\n███░░░░░░░░░░███"
                                  "\n████████████████"
                                  "\n████████████████");
                                  */
  // CHECK(c.string("░", "█") == expected);
  //\todo Fix rendering of filled rounded rectangles
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing triangles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.drawTriangle(0, 0, 15, 0, 0, 8, {0xff});
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n█░░░░░░░░░░░░██░"
                                  "\n█░░░░░░░░░░██░░░"
                                  "\n█░░░░░░░░██░░░░░"
                                  "\n█░░░░░░██░░░░░░░"
                                  "\n█░░░░██░░░░░░░░░"
                                  "\n█░░██░░░░░░░░░░░"
                                  "\n███░░░░░░░░░░░░░"
                                  "\n█░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledTriangle(0, 0, 15, 0, 0, 8, {0xff}, {0xff});
  CHECK(CTH::displayContent(c) == "\n████████████████"
                                  "\n███████████████░"
                                  "\n█████████████░░░"
                                  "\n███████████░░░░░"
                                  "\n█████████░░░░░░░"
                                  "\n███████░░░░░░░░░"
                                  "\n█████░░░░░░░░░░░"
                                  "\n███░░░░░░░░░░░░░"
                                  "\n█░░░░░░░░░░░░░░░");

  c.black();
  c.drawFilledTriangle(0, 0, 15, 0, 0, 8, {0xff}, {0xff});
  c.drawFilledTriangle(15, 8, 0, 0, 0, 8, {util::ColorRGB::BlendMode::Invert}, {util::ColorRGB::BlendMode::Invert});
  CHECK(CTH::displayContent(c) == "\n░███████████████"
                                  "\n░░█████████████░"
                                  "\n░░░░█████████░░░"
                                  "\n░░░░░░█████░░░░░"
                                  "\n░░░░░░░░█░░░░░░░"
                                  "\n░░░░░░░███░░░░░░"
                                  "\n░░░░░███████░░░░"
                                  "\n░░░███████████░░"
                                  "\n░███████████████");
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Canvas: drawing circles", "[gfx/Canvas]")
{
  Canvas c(16, 9);
  std::string expected;

  c.black();
  c.drawCircle(8, 4, 4, {0xff});
  CHECK(CTH::displayContent(c) == "\n░░░░░░░███░░░░░░"
                                  "\n░░░░░██░░░██░░░░"
                                  "\n░░░░░█░░░░░█░░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░█░░░░░░░█░░░"
                                  "\n░░░░░█░░░░░█░░░░"
                                  "\n░░░░░██░░░██░░░░"
                                  "\n░░░░░░░███░░░░░░");

  c.white();
  c.drawFilledCircle(8, 4, 4, {0x00}, {0x00});
  CHECK(CTH::displayContent(c) == "\n███████░░░██████"
                                  "\n█████░░░░░░░████"
                                  "\n█████░░░░░░░████"
                                  "\n████░░░░░░░░░███"
                                  "\n████░░░░░░░░░███"
                                  "\n████░░░░░░░░░███"
                                  "\n█████░░░░░░░████"
                                  "\n█████░░░░░░░████"
                                  "\n███████░░░██████");
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
