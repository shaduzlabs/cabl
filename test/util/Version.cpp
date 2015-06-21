/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include "catch.hpp"

#include <util/Version.h>

namespace sl
{
namespace util
{
namespace test
{

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "Null vs non-null version", "[util/version]" ) {
  Version version_empty;
  Version version_nonNull_1(1);
  Version version_nonNull_2(1,2);
  Version version_nonNull_3(1,2,3);
  
  CHECK_FALSE(version_empty);
  CHECK(version_nonNull_1);
  CHECK(version_nonNull_2);
  CHECK(version_nonNull_3);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "Constructor, getters and setters", "[util/version]" ) {
  Version version_952(9,5,2);
  Version version_95(9,5);
  Version version_9(9);
  
  Version version_empty;
  
  CHECK( version_952.getMajor() == 9 );
  CHECK( version_952.getMinor() == 5 );
  CHECK( version_952.getMicro() == 2 );

  CHECK( version_95.getMajor() == 9 );
  CHECK( version_95.getMinor() == 5 );
  CHECK( version_95.getMicro() == 0 );

  CHECK( version_9.getMajor() == 9 );
  CHECK( version_9.getMinor() == 0 );
  CHECK( version_9.getMicro() == 0 );
  
  CHECK( version_empty.getMajor() == 0 );
  CHECK( version_empty.getMinor() == 0 );
  CHECK( version_empty.getMicro() == 0 );

  version_empty.setMajor(11);
  CHECK( version_empty.getMajor() == 11 );
  CHECK( version_empty.getMinor() == 0 );
  CHECK( version_empty.getMicro() == 0 );

  version_empty.setMinor(55);
  CHECK( version_empty.getMajor() == 11 );
  CHECK( version_empty.getMinor() == 55 );
  CHECK( version_empty.getMicro() == 0 );

  version_empty.setMicro(91);
  CHECK( version_empty.getMajor() == 11 );
  CHECK( version_empty.getMinor() == 55 );
  CHECK( version_empty.getMicro() == 91 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "Comparison", "[util/version]" ) {
  Version version_1_2_3(1,2,3);
  Version version_1_2_4(1,2,4);
  Version version_1_2(1,2);
  Version version_1_3(1,3);
  Version version_1(1);
  
  Version version_null;
  
  CHECK(version_1 >= version_1_3);
}

//----------------------------------------------------------------------------------------------------------------------

} // test
} // util
} // sl