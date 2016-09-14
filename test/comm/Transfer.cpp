/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#ifdef CABL_USE_NETWORK
#include <cereal/archives/portable_binary.hpp>
#endif

#include <sstream>

#include <cabl/comm/Transfer.h>

namespace sl
{
namespace cabl
{
namespace test
{


//--------------------------------------------------------------------------------------------------

TEST_CASE("Constructors, reset, bool operator", "[comm][Transfer]")
{
  Transfer t1;
  CHECK(t1.size() == 0);
  CHECK_FALSE(t1);

  Transfer t2(19);
  CHECK(t2.size() == 19);
  CHECK(t2);

  Transfer t3({0, 1, 2, 3, 4, 5, 6});
  CHECK(t3.size() == 7);
  CHECK(t3);

  Transfer t4({0, 1, 2, 3}, {4, 5, 6, 7});
  CHECK(t4.size() == 8);
  CHECK(t4);

  tRawData data{3, 4, 5, 6, 7, 8};
  Transfer t5({0, 1, 2}, data.data(), data.size());
  CHECK(t5.size() == 9);
  CHECK(t5);

  t5.reset();
  CHECK(t5.size() == 0);
  CHECK_FALSE(t5);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Transfer comparison", "[comm][Transfer]")
{
  Transfer t1({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  Transfer t2({0, 1, 2, 3, 4, 5, 6, 7, 8});
  Transfer t3({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  Transfer t4({1, 0, 2, 3, 4, 5, 6, 7, 8, 9});
  Transfer t5({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  CHECK_FALSE(t1 == t2);
  CHECK_FALSE(t1 == t3);
  CHECK_FALSE(t1 == t4);
  CHECK_FALSE(t1 != t5);

  t1.setData(nullptr, 20);
  CHECK(t1 == t5);

  t1.setData(t4.data().data(), 0);
  CHECK(t1 == t5);

  t1.setData(t4.data().data(), t4.size());
  CHECK(t1 != t5);
  CHECK(t1 == t4);
  CHECK(t1.size() == t4.size());
}

//--------------------------------------------------------------------------------------------------
#ifdef CABL_USE_NETWORK

TEST_CASE("Serialization/deserialization", "[comm][Transfer]")
{
  std::stringstream ss;
  Transfer original({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  cereal::PortableBinaryOutputArchive oArchive(ss);
  oArchive(original);

  cereal::PortableBinaryInputArchive iArchive(ss);
  Transfer deserialized;
  iArchive(deserialized);

  CHECK(deserialized == original);
}

#endif

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
