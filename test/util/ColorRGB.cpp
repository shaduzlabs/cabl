/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <util/ColorRGB.h>

namespace sl
{
namespace util
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: constructors", "[util/ColorRGB]")
{
  ColorRGB color;
  CHECK(color.red() == 0);
  CHECK(color.green() == 0);
  CHECK(color.blue() == 0);
  CHECK(color.mono() == 0);

  ColorRGB color2(18);
  CHECK(color2.red() == 18);
  CHECK(color2.green() == 18);
  CHECK(color2.blue() == 18);
  CHECK(color2.mono() == 18);

  ColorRGB color3(20, 40, 60);
  CHECK(color3.red() == 20);
  CHECK(color3.green() == 40);
  CHECK(color3.blue() == 60);
  CHECK(color3.mono() == 60);

  ColorRGB color4(20, 40, 60, 80);
  CHECK(color4.red() == 20);
  CHECK(color4.green() == 40);
  CHECK(color4.blue() == 60);
  CHECK(color4.mono() == 80);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: getters/setters", "[util/ColorRGB]")
{
  ColorRGB color;

  color.setRed(22);
  color.setGreen(33);
  color.setBlue(44);
  color.setMono(55);
  CHECK(color.red() == 22);
  CHECK(color.green() == 33);
  CHECK(color.blue() == 44);
  CHECK(color.mono() == 55);
  CHECK(color.getValue() == 0x16212C);

  color.black();
  CHECK(color.red() == 0);
  CHECK(color.green() == 0);
  CHECK(color.blue() == 0);
  CHECK(color.mono() == 0);
  CHECK(color.getValue() == 0);

  color.white();
  CHECK(color.red() == 255);
  CHECK(color.green() == 255);
  CHECK(color.blue() == 255);
  CHECK(color.mono() == 255);
  CHECK(color.getValue() == 0xFFFFFF);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: distance", "[util/ColorRGB]")
{
  ColorRGB color1;
  ColorRGB color2;
  ColorRGB color3(127);

  color1.black();
  color2.white();
  CHECK(color2.distance(color1) == color1.distance(color2));
  CHECK(color2.distance(color1) > color3.distance(color2));
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: comparison", "[util/ColorRGB]")
{
  ColorRGB color1a(12);
  ColorRGB color1b(12);
  ColorRGB color2(127);

  CHECK(color1a == color1b);
  CHECK(color2 != color1a);
  CHECK(color1a <= color2);
  CHECK(color1a < color2);
  CHECK(color2 > color1a);
  CHECK(color2 >= color1a);
  CHECK(color2 >= color2);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: streaming", "[util/ColorRGB]")
{
  ColorRGB color(11, 33, 55, 77);
  std::stringstream os;
  os << color;
  CHECK(os.str() == "11,33,55,77");
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ColorRGB: blend modes", "[util/ColorRGB]")
{
  ColorRGB color(11, 33, 55, 77);
  
  color.setBlendMode(ColorRGB::BlendMode::Invert);
  
  CHECK(color.red() == static_cast<uint8_t>(~(11)));
  CHECK(color.green() == static_cast<uint8_t>(~(33)));
  CHECK(color.blue() == static_cast<uint8_t>(~(55)));
  CHECK(color.mono() == static_cast<uint8_t>(~(77)));
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace util
} // namespace sl
