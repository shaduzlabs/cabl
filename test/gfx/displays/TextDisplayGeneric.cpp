/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <gfx/displays/TextDisplayGeneric.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("TextDisplayGeneric: constructor", "[gfx][displays][TextDisplayGeneric]")
{
  TextDisplayGeneric<8, 4> display;
  CHECK(display.width() == 8);
  CHECK(display.height() == 4);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("TextDisplayGeneric: set text", "[gfx][displays][TextDisplayGeneric]")
{
  TextDisplayGeneric<130, 4> display;

  SECTION("Write text")
  {
    display.putText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
                 "tempor incididunt ut labore et dolore magna aliqua. Ut",
      0,
      Alignment::Left);
    display.putText("enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
                 "aliquip ex ea commodo consequat. Duis aute irure dolor ",
      1,
      Alignment::Left);
    display.putText("in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
                 "pariatur. Excepteur sint occaecat cupidatat non proident,",
      2,
      Alignment::Left);
    display.putText(
      "sunt in culpa qui officia deserunt mollit anim id est laborum.", 3, Alignment::Left);

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
