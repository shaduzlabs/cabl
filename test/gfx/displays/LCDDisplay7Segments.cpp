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
#include <gfx/displays/LCDDisplay7Segments.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

template class LCDDisplayBase<19, 1>;
template class LCDDisplay7Segments<19>;

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

TEST_CASE("LCDDisplay7Segments: set text", "[gfx][displays][LCDDisplay7Segments]")
{
  LCDDisplay7Segments<19> display;

  {
    display.setText("@A text!@", 0, LCDDisplay::Align::Center);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0, 0, 0, 0, 0, 0, 0x7E, 0, 0xE2, 0xF2, 0, 0xE2, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.setText("      ", 0, LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.setText("                    ! ? *", 0, LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.setText("AAA", 0, LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0x7E, 0x7E, 0x7E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.setText("AAA", 0, LCDDisplay::Align::Right);
    std::vector<uint8_t> displayData(
      display.displayData(), display.displayData() + display.dataSize());
    std::vector<uint8_t> expected
      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7E, 0x7E, 0x7E};
    CHECK(displayData == expected);
    display.clear();
  }
  {
    display.setText("AAA", 0, LCDDisplay::Align::Center);
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
