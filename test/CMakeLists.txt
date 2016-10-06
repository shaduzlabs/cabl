
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
    comm/DeviceDescriptor.cpp
    comm/DiscoveryPolicy.cpp
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
    gfx/displays/TextDisplay7Segments.cpp
    gfx/displays/TextDisplayGeneric.cpp
    gfx/displays/TextDisplayKompleteKontrol.cpp
    gfx/displays/NullCanvas.cpp
    gfx/displays/GDisplayMaschineMikro.cpp
    gfx/displays/GDisplayMaschineMK1.cpp
    gfx/displays/GDisplayMaschineMK2.cpp
    gfx/displays/GDisplayPush2.cpp
)

set(
  test_util_SRCS
    util/Color.cpp
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

target_include_directories(${PROJECT_NAME} PRIVATE ${CATCH_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR} ${CABL_ROOT_DIR}/src)

target_link_libraries(${PROJECT_NAME} cabl-static lodepng)
set(CATCH_CMDARGS "")
if(CABL_TEST_ALL)
  set(CATCH_CMDARGS "${CATCH_CMDARGS} -s")
endif()
if(CABL_TEST_JUNIT)
  set(CATCH_CMDARGS "${CATCH_CMDARGS} -r junit -o test-report.junit.xml")
else()
  set(CATCH_CMDARGS "${CATCH_CMDARGS} -r compact")
endif()

if(WIN32)
  separate_arguments(CATCH_CMDARGS_LIST WINDOWS_COMMAND "${CATCH_CMDARGS}")
else()
  separate_arguments(CATCH_CMDARGS_LIST UNIX_COMMAND "${CATCH_CMDARGS}")
endif()

add_test( NAME ${PROJECT_NAME} WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND ${PROJECT_NAME} ${CATCH_CMDARGS_LIST})

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C ${CMAKE_BUILD_TYPE} DEPENDS ${PROJECT_NAME})
