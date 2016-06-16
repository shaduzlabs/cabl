
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Unit tests                                                                                      #
# ------------------------------------------------------------------------------------------------ #

project(unit-tests)
enable_testing(true)

# Dependencies ----------------------------------------------------------------------------------- #
addCatch()

# Unit tests files ------------------------------------------------------------------------------- #
set(
  test_SRCS
    main.cpp
)

set(
  test_util_SRCS
    util/Version.cpp
)

source_group(""                  FILES ${test_SRCS})
source_group("util"              FILES ${test_util_SRCS})

set(
  Test_FILES
    ${test_SRCS}
    ${test_util_SRCS}
)

add_executable(
  ${PROJECT_NAME}
  ${Test_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH_INCLUDE_DIR} ${UNMIDIFY_INCLUDE_DIR})

add_test( ${PROJECT_NAME} ${PROJECT_NAME} )

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${PROJECT_NAME})
