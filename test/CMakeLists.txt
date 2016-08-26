
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Unit tests                                                                                      #
# ------------------------------------------------------------------------------------------------ #

include_directories( ${CATCH_INCLUDE_DIRS} )

project(unit-tests)

set(
  test_SRCS
    main.cpp
)

set(
  test_comm_SRCS
    comm/Transfer.cpp
)

set(
  test_util_SRCS
    util/Version.cpp
)

source_group(""                  FILES ${test_SRCS})
source_group("comm"              FILES ${test_comm_SRCS})
source_group("util"              FILES ${test_util_SRCS})

set(
  Test_FILES
    ${test_SRCS}
    ${test_comm_SRCS}
    ${test_util_SRCS}
)

add_executable(
  ${PROJECT_NAME}
  ${Test_FILES}
)

if(COVERALLS)
  set_target_properties ( ${PROJECT_NAME} PROPERTIES LINK_FLAGS " -Wall -fprofile-arcs -ftest-coverage")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} cabl-static)

add_test( ${PROJECT_NAME} ${PROJECT_NAME} )

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${PROJECT_NAME})
