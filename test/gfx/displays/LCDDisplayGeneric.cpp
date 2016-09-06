/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/LCDDisplayGeneric.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("LCDDisplayGeneric: constructor", "[gfx][displays][LCDDisplayGeneric]")
{
  LCDDisplayGeneric<8, 4> display;
  CHECK(display.width() == 8);
  CHECK(display.height() == 4);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("LCDDisplayGeneric: set text", "[gfx][displays][LCDDisplayGeneric]")
{
  LCDDisplayGeneric<130, 4> display;

  SECTION("Write text")
  {
    display.setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
                    "tempor incididunt ut labore et dolore magna aliqua. Ut",
      0,
      LCDDisplay::Align::Left);
    display.setText("enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
                    "aliquip ex ea commodo consequat. Duis aute irure dolor ",
      1,
      LCDDisplay::Align::Left);
    display.setText("in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
                    "pariatur. Excepteur sint occaecat cupidatat non proident,",
      2,
      LCDDisplay::Align::Left);
    display.setText(
      "sunt in culpa qui officia deserunt mollit anim id est laborum.", 3, LCDDisplay::Align::Left);

    std::string displayData(display.displayData(), display.displayData() + display.dataSize());

    std::string expected{
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt "
      "ut labore et dolore magna aliqua. Ut    enim ad minim veniam, quis nostrud exercitation "
      "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor    in "
      "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur "
      "sint occaecat cupidatat non proident,  sunt in culpa qui officia deserunt mollit anim id "
      "est laborum.                                                                    "};
    CHECK(displayData == expected);
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
