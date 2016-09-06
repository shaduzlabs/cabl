
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Unit tests                                                                                      #
# ------------------------------------------------------------------------------------------------ #

include_directories( ${CATCH_INCLUDE_DIRS} )

addlodepng()

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
  test_gfx_SRCS
    gfx/Canvas.cpp
    gfx/CanvasTestFunctions.cpp
    gfx/CanvasTestFunctions.h
    gfx/CanvasTestHelpers.cpp
    gfx/CanvasTestHelpers.h
)

set(
  test_gfx_displays_SRCS
    gfx/displays/LCDDisplay7Segments.cpp
    gfx/displays/LCDDisplayGeneric.cpp
    gfx/displays/LCDDisplayKompleteKontrol.cpp
    gfx/displays/GDisplayDummy.cpp
    gfx/displays/GDisplayMaschineMikro.cpp
    gfx/displays/GDisplayMaschineMK1.cpp
    gfx/displays/GDisplayMaschineMK2.cpp
    gfx/displays/GDisplayPush2.cpp
)

set(
  test_util_SRCS
    util/ColorRGB.cpp
    util/Version.cpp
)

source_group(""                  FILES ${test_SRCS})
source_group("comm"              FILES ${test_comm_SRCS})
source_group("gfx"               FILES ${test_gfx_SRCS})
source_group("gfx\\displays"     FILES ${test_gfx_displays_SRCS})
source_group("util"              FILES ${test_util_SRCS})

set(
  Test_FILES
    ${test_SRCS}
    ${test_comm_SRCS}
    ${test_gfx_SRCS}
    ${test_gfx_displays_SRCS}
    ${test_util_SRCS}
)

add_executable(
  ${PROJECT_NAME}
  ${Test_FILES}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CABL_ROOT_DIR}/support/test-data $<TARGET_FILE_DIR:${PROJECT_NAME}>/test-data
)

if(COVERALLS)
  set_target_properties ( ${PROJECT_NAME} PROPERTIES LINK_FLAGS " -Wall -fprofile-arcs -ftest-coverage")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} cabl-static lodepng)

add_test( ${PROJECT_NAME} ${PROJECT_NAME} )

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure DEPENDS ${PROJECT_NAME})
