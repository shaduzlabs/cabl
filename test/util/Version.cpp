/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <cabl/util/Version.h>

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

TEST_CASE("Version: null vs non-null, conversion to bool", "[util][Version]")
{
  Version version_empty;
  Version version_nonNull_1(1);
  Version version_nonNull_2(1, 2);
  Version version_nonNull_3(1, 2, 3);

  CHECK_FALSE(version_empty);
  CHECK(version_nonNull_1);
  CHECK(version_nonNull_2);
  CHECK(version_nonNull_3);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Version: constructor, getters and setters", "[util][Version]")
{
  Version version_952(9, 5, 2);
  Version version_95(9, 5);
  Version version_9(9);

  Version version_empty;

  CHECK(version_952.getMajor() == 9);
  CHECK(version_952.getMinor() == 5);
  CHECK(version_952.getMicro() == 2);

  CHECK(version_95.getMajor() == 9);
  CHECK(version_95.getMinor() == 5);
  CHECK(version_95.getMicro() == 0);

  CHECK(version_9.getMajor() == 9);
  CHECK(version_9.getMinor() == 0);
  CHECK(version_9.getMicro() == 0);

  CHECK(version_empty.getMajor() == 0);
  CHECK(version_empty.getMinor() == 0);
  CHECK(version_empty.getMicro() == 0);

  version_empty.setMajor(11);
  CHECK(version_empty.getMajor() == 11);
  CHECK(version_empty.getMinor() == 0);
  CHECK(version_empty.getMicro() == 0);

  version_empty.setMinor(55);
  CHECK(version_empty.getMajor() == 11);
  CHECK(version_empty.getMinor() == 55);
  CHECK(version_empty.getMicro() == 0);

  version_empty.setMicro(91);
  CHECK(version_empty.getMajor() == 11);
  CHECK(version_empty.getMinor() == 55);
  CHECK(version_empty.getMicro() == 91);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Version: reset", "[util][Version]")
{
  Version version_1_2_3(1, 2, 3);

  CHECK(version_1_2_3);

  version_1_2_3.reset();
  CHECK_FALSE(version_1_2_3);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Version: conversion to string", "[util][Version]")
{
  Version version_1_2_3(1, 2, 3);
  Version version_1_2(1, 2);
  Version version_1(1);

  Version version_empty;

  CHECK(version_1_2_3.toString() == "1.2.3");
  CHECK(version_1_2.toString() == "1.2.0");
  CHECK(version_1.toString() == "1.0.0");

  CHECK(static_cast<std::string>(version_empty) == "0.0.0");
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Version: version comparison", "[util][Version]")
{
  Version version_2_3_4(2, 3, 4);
  Version version_1_2_3(1, 2, 3);
  Version version_1_2_4(1, 2, 4);
  Version version_1_2(1, 2);
  Version version_1_3(1, 3);
  Version version_1(1);

  Version version_null;

  CHECK(version_1 < version_1_2);
  CHECK(version_1 < version_1_2_3);
  CHECK(version_1 >= version_null);
  CHECK(version_null <= version_1);
  CHECK(version_1 > version_null);
  CHECK(version_1 != version_null);
  CHECK(version_1 != version_1_2);
  CHECK(version_1 != version_1_2_3);
  CHECK(version_1_2 <= version_1_2);
  CHECK(version_1_2 <= version_1_3);
  CHECK(version_1_2_3 < version_2_3_4);
  CHECK(version_2_3_4 > version_1_2_3);
  CHECK_FALSE(version_1 > version_1_2);
  CHECK_FALSE(version_1 == version_1_2);
  CHECK_FALSE(version_1 == version_1_2_3);
  CHECK_FALSE(version_1 >= version_1_3);
  CHECK_FALSE(version_1 == version_null);

  CHECK(version_1_2_4 > version_1_2_3);
  CHECK(version_1_2_3 < version_1_2_4);
  CHECK(version_1_2_4 >= version_1_2_3);
  CHECK(version_1_2_3 <= version_1_2_4);
  CHECK(version_1_2_3 <= version_1_2_3);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
