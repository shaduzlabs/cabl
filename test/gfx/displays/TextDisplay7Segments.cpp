/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <algorithm>
#include <iostream>
#include <iterator>

#include <catch.hpp>
#include <gfx/displays/TextDisplay7Segments.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("TextDisplay7Segments: constructor", "[gfx][displays][TextDisplay7Segments]")
{
  TextDisplay7Segments<19> display;
  CHECK(display.width() == 19);
  CHECK(display.height() == 1);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("TextDisplay7Segments: set text", "[gfx][displays][TextDisplay7Segments]")
{
  TextDisplay7Segments<19> display;

  {
    display.putText("@A text!@", 0, Alignment::Center);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0, 0, 0, 0, 0, 0, 0x7E, 0, 0xE2, 0xF2, 0, 0xE2, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.putText("      ", 0, Alignment::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.putText("                    ! ? *", 0, Alignment::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.putText("AAA", 0, Alignment::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0x7E, 0x7E, 0x7E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.putText("AAA", 0, Alignment::Right);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7E, 0x7E, 0x7E};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.putText("AAA", 0, Alignment::Center);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0, 0, 0, 0, 0, 0, 0, 0, 0x7E, 0x7E, 0x7E, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
