/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <iostream>
#include <iterator>
#include <algorithm>

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

TEST_CASE("LCDDisplay7Segments: set text", "[gfx][displays][LCDDisplay7Segments]")
{
  LCDDisplay7Segments<19> display;
  
  {
    display.setText(" A text! ",0,LCDDisplay::Align::Center);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {97, 97, 97, 97, 97, 97, 126, 97, 226, 242, 0, 226, 121, 97, 97, 97, 97, 97, 97};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
  {
    display.setText("      ",0,LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
  {
    display.setText("                    ! ? *",0,LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
  {
    display.setText("!!!",0,LCDDisplay::Align::Left);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {121, 121, 121, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
  {
    display.setText("!!!",0,LCDDisplay::Align::Right);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 121, 121, 121};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
  {
    display.setText("!!!",0,LCDDisplay::Align::Center);
    std::vector<uint8_t> displayData(display.displayData(), display.displayData()+display.dataSize());
    std::vector<uint8_t> expected = {97, 97, 97, 97, 97, 97, 97, 97, 121, 121, 121, 97, 97, 97, 97, 97, 97, 97, 97};
    CHECK( std::equal( displayData.begin(), displayData.end(), std::begin(expected)) );
    display.clear();
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
