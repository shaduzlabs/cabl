
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Unit tests                                                                                      #
# ------------------------------------------------------------------------------------------------ #

include_directories( ${CATCH_INCLUDE_DIR} )

project(unit-tests)

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

add_test(NAME ${PROJECT_NAME}-runner
        COMMAND ${PROJECT_NAME} -s -b
        CONFIGURATIONS Debug Release
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

target_link_libraries(${PROJECT_NAME} cabl-static)
