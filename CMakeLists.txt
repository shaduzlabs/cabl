
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 2.8.0)

project (cabl)

set(CABL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CABL_BUILD_DIR ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${CABL_ROOT_DIR}/cmake-modules")
include(sl-main)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  list(APPEND CMAKE_MODULE_PATH "${CABL_ROOT_DIR}/support/windows/cmake")
  include(GenerateProductVersion)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set(LIBRARY_FILE_NAME     "cabl")
set(LIBRARY_FOLDER_NAME   "cabl")
set(DEBUG_SUFFIX          "_d")

# Options ---------------------------------------------------------------------------------------- #

option(BUILD_SHARED_LIBS "build as shared library" OFF)
option(BUILD_STATIC_LIBS "build as static library" ON)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  set(IS_CABL ON)
else()
  set(IS_CABL OFF)
endif()

option(CABL_FORCE_BUILD_DEPENDENCIES   "Force download and build dependencies" OFF)

option(CABL_TEST      "Build cabl tests"     ${IS_CABL})
option(CABL_EXAMPLES  "Build cabl examples"  ${IS_CABL})
option(CABL_DOCS      "Build cabl docs"      ${IS_CABL})
option(CABL_PYTHON    "Build python binding" ${IS_CABL})

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

# Version ---------------------------------------------------------------------------------------- #

string(TIMESTAMP CABL_BUILD_DATE "%Y-%m-%d")

set (CABL_VERSION_MAJOR 0)
set (CABL_VERSION_MINOR 9)
set (CABL_VERSION_MICRO 3)
set (CABL_VERSION_STRING "${CABL_VERSION_MAJOR}.${CABL_VERSION_MINOR}.${CABL_VERSION_MICRO}")

if($ENV{APPVEYOR})
  set (CABL_BUILD_HOST "Appveyor")
  set (CABL_BUILD_NUMBER ".$ENV{APPVEYOR_BUILD_NUMBER}")
  set (CABL_GIT_BRANCH $ENV{APPVEYOR_REPO_BRANCH})
  execute_process(COMMAND appveyor UpdateBuild -Version ${CABL_VERSION_STRING})
elseif($ENV{TRAVIS})
  set (CABL_BUILD_HOST "Travis-CI")
  set (CABL_BUILD_NUMBER ".$ENV{TRAVIS_BUILD_NUMBER}")
  set (CABL_GIT_BRANCH $ENV{TRAVIS_BRANCH})
else()
  set (CABL_BUILD_HOST "a local machine")
  set (CABL_BUILD_NUMBER "")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CABL_ROOT_DIR}
    OUTPUT_VARIABLE CABL_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

if(${CABL_GIT_BRANCH} STREQUAL "master")
  set(CABL_BUILD_TYPE "release version")
  set (CABL_VERSION_STRING "${CABL_VERSION_STRING}${CABL_BUILD_NUMBER}")
elseif(${CABL_GIT_BRANCH} STREQUAL "develop")
  set(CABL_BUILD_TYPE "development version")
  set (CABL_VERSION_STRING "${CABL_VERSION_STRING}${CABL_BUILD_NUMBER}-dev")
else()
  set(CABL_BUILD_TYPE "development version from feature branch: ${GIT_BRANCH}")
  set (CABL_VERSION_STRING "${CABL_VERSION_STRING}${CABL_BUILD_NUMBER}-fb")
endif()

configure_file (
  "${CABL_ROOT_DIR}/inc/cabl-config.h.in"
  "${PROJECT_BINARY_DIR}/cabl-config.h"
)

if($ENV{APPVEYOR})
  execute_process(COMMAND appveyor UpdateBuild -Version ${CABL_VERSION_STRING})
  set(ENV{APPVEYOR_BUILD_VERSION} "${CABL_VERSION_STRING}")
elseif($ENV{TRAVIS})
  set(CABL_DEPLOY_TARGET $ENV{CABL_DEPLOY_TARGET})
  if(CABL_DEPLOY_TARGET)
    message(STATUS "Package ${CABL_DEPLOY_TARGET} will be deployed")
    configure_file (
      "${CABL_ROOT_DIR}/support/travis/deploy-bintray.json.in"
      "${PROJECT_BINARY_DIR}/deploy-bintray.json"
    )
  endif()

  set(COVERAGE_BUILD $ENV{COVERAGE_BUILD})
  if(COVERAGE_BUILD)
    set(COVERALLS ${COVERAGE_BUILD})
  endif()
endif()

set(CABL_BUILD_DESCRIPTION "cabl v. ${CABL_VERSION_STRING} - ${CABL_BUILD_TYPE} built on ${CABL_BUILD_HOST}")

MESSAGE( STATUS "                                   ")
MESSAGE( STATUS "                            __       ___       ")
MESSAGE( STATUS "                           /\\ \\     /\\_ \\      ")
MESSAGE( STATUS "               ___     __  \\ \\ \\____\\//\\ \\     ")
MESSAGE( STATUS "              /'___\\ /'__`\\ \\ \\ '__`\\ \\ \\ \\    ")
MESSAGE( STATUS "             /\\ \\__//\\ \\L\\.\\_\\ \\ \\L\\ \\ \\_\\ \\_  ")
MESSAGE( STATUS "             \\ \\____\\ \\__/.\\_\\\\ \\_,__/ /\\____\\ ")
MESSAGE( STATUS "              \\/____/\\/__/\\/_/ \\/___/  \\/____/ ")
MESSAGE( STATUS "               Controller ABstraction Library                      ")

MESSAGE( STATUS "                                  v. ${CABL_VERSION_STRING}")
MESSAGE( STATUS "                                               ")
message( STATUS "[${CABL_BUILD_DATE}] ${CABL_BUILD_DESCRIPTION} ")

# Dependencies ----------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR ( ${CABL_FORCE_BUILD_DEPENDENCIES} AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin") )
  addLibUSB()
else()
  find_package(LibUSB REQUIRED)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR ${CABL_FORCE_BUILD_DEPENDENCIES})
  addHIDAPI()
  addRtMidi()
else()
  find_package(HIDAPI REQUIRED)
  find_package(RtMidi REQUIRED)
endif()

addUnmidify()

# If Boost.python is not available, just display a warning and skip the python wrapper target
if(${CABL_PYTHON})
  find_package(Boost COMPONENTS python)
  if(Boost_PYTHON_FOUND)
    find_package(PythonLibs REQUIRED)
  else()
    message(WARNING "Boost.python has not been found, the python wrapper will not be created.")
    set(CABL_PYTHON OFF)
  endif()
endif()

# If Doxygen is not available, just display a warning and skip the documentation target
if(${CABL_DOCS})
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen has not been found, the documentation will not be created.")
    set(CABL_DOCS OFF)
  endif()
endif()

# OSX specific ----------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_SKIP_BUILD_RPATH OFF)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Includes --------------------------------------------------------------------------------------- #

include_directories("${PROJECT_BINARY_DIR}")

set(
  inc_client_INCLUDES
    inc/client/Client.h
)

set(
  inc_comm_INCLUDES
    inc/comm/DeviceDescriptor.h
    inc/comm/DeviceHandle.h
    inc/comm/DiscoveryPolicy.h
    inc/comm/Driver.h
    inc/comm/Transfer.h
)

set(
  inc_devices_INCLUDES
    inc/devices/Coordinator.h
    inc/devices/Device.h
    inc/devices/DeviceFactory.h
    inc/devices/DeviceRegistrar.h
)

set(
  inc_gfx_INCLUDES
    inc/gfx/Canvas.h
    inc/gfx/Font.h
    inc/gfx/FontManager.h
    inc/gfx/GDisplay.h
    inc/gfx/LCDDisplay.h
)

set(
  inc_util_INCLUDES
    inc/util/ColorRGB.h
    inc/util/Functions.h
    inc/util/Log.h
    inc/util/Macros.h
    inc/util/Types.h
    inc/util/Version.h
)

set(
  inc_INCLUDES
    inc/cabl.h
)

source_group("inc\\client"           FILES ${inc_client_INCLUDES})
source_group("inc\\comm"             FILES ${inc_comm_INCLUDES})
source_group("inc\\devices"          FILES ${inc_devices_INCLUDES})
source_group("inc\\gfx"              FILES ${inc_gfx_INCLUDES})
source_group("inc\\util"             FILES ${inc_util_INCLUDES})
source_group("inc"                   FILES ${inc_INCLUDES})

set (
  cabl_INCLUDES
    ${inc_client_INCLUDES}
    ${inc_comm_INCLUDES}
    ${inc_devices_INCLUDES}
    ${inc_devices_ableton_INCLUDES}
    ${inc_devices_akai_INCLUDES}
    ${inc_devices_generic_INCLUDES}
    ${inc_devices_ni_INCLUDES}
    ${inc_gfx_displays_INCLUDES}
    ${inc_gfx_INCLUDES}
    ${inc_util_INCLUDES}
    ${inc_INCLUDES}
    ${VERSION_RESOURCES}
)


# Source files ----------------------------------------------------------------------------------- #

set(
  src_client_SRCS
    src/client/Client.cpp
)

set(
  src_comm_drivers_HIDAPI_SRCS
    src/comm/drivers/HIDAPI/DriverHIDAPI.cpp
    src/comm/drivers/HIDAPI/DriverHIDAPI.h
    src/comm/drivers/HIDAPI/DeviceHandleHIDAPI.cpp
    src/comm/drivers/HIDAPI/DeviceHandleHIDAPI.h
)

set(
  src_comm_drivers_LibUSB_SRCS
    src/comm/drivers/LibUSB/DriverLibUSB.cpp
    src/comm/drivers/LibUSB/DriverLibUSB.h
    src/comm/drivers/LibUSB/DeviceHandleLibUSB.cpp
    src/comm/drivers/LibUSB/DeviceHandleLibUSB.h
)

set(
  src_comm_drivers_MAX3421E_SRCS
    src/comm/drivers/MAX3421E/DriverMAX3421E.cpp
    src/comm/drivers/MAX3421E/DriverMAX3421E.h
    src/comm/drivers/MAX3421E/DeviceHandleMAX3421E.cpp
    src/comm/drivers/MAX3421E/DeviceHandleMAX3421E.h
)

set(
  src_comm_drivers_MIDI_SRCS
    src/comm/drivers/MIDI/DriverMIDI.cpp
    src/comm/drivers/MIDI/DriverMIDI.h
    src/comm/drivers/MIDI/DeviceHandleMIDI.cpp
    src/comm/drivers/MIDI/DeviceHandleMIDI.h
)

set(
  src_comm_drivers_Probe_SRCS
    src/comm/drivers/Probe/DriverProbe.cpp
    src/comm/drivers/Probe/DriverProbe.h
    src/comm/drivers/Probe/DeviceHandleProbe.cpp
    src/comm/drivers/Probe/DeviceHandleProbe.h
)

set(
  src_comm_drivers_SAM3X8E_SRCS
    src/comm/drivers/SAM3X8E/DriverSAM3X8E.cpp
    src/comm/drivers/SAM3X8E/DriverSAM3X8E.h
    src/comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.cpp
    src/comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h
)

set(
  src_comm_SRCS
    src/comm/DeviceHandle.cpp
    src/comm/DeviceHandleImpl.h
    src/comm/DiscoveryPolicy.cpp
    src/comm/Driver.cpp
    src/comm/DriverImpl.h
    src/comm/Transfer.cpp
)

set(
  src_devices_SRCS
    src/devices/Coordinator.cpp
    src/devices/DeviceFactory.cpp
)

set(
  src_devices_ableton_SRCS
    src/devices/ableton/Push2.h
    src/devices/ableton/Push2.cpp
    src/devices/ableton/Push2Display.h
    src/devices/ableton/Push2Display.cpp
)

set(
  src_devices_akai_SRCS
    src/devices/akai/Push.h
    src/devices/akai/Push.cpp
)

set(
  src_devices_generic_SRCS
    src/devices/generic/USBMidi.h
    src/devices/generic/USBMidi.cpp
)

set(
  src_devices_ni_SRCS
    src/devices/ni/KompleteKontrol.h
    src/devices/ni/KompleteKontrol.cpp
    src/devices/ni/MaschineMK1.h
    src/devices/ni/MaschineMK1.cpp
    src/devices/ni/MaschineMK2.h
    src/devices/ni/MaschineMK2.cpp
    src/devices/ni/MaschineMikroMK2.h
    src/devices/ni/MaschineMikroMK2.cpp
    src/devices/ni/TraktorF1MK2.h
    src/devices/ni/TraktorF1MK2.cpp
)

set(
  src_gfx_displays_SRCS
    src/gfx/displays/GDisplayDummy.h
    src/gfx/displays/GDisplayMaschineMikro.h
    src/gfx/displays/GDisplayMaschineMikro.cpp
    src/gfx/displays/GDisplayMaschineMK1.h
    src/gfx/displays/GDisplayMaschineMK1.cpp
    src/gfx/displays/GDisplayMaschineMK2.h
    src/gfx/displays/GDisplayMaschineMK2.cpp
    src/gfx/displays/GDisplayPush2.h
    src/gfx/displays/GDisplayPush2.cpp
    src/gfx/displays/LCDDisplay7Segments.h
    src/gfx/displays/LCDDisplayGeneric.h
    src/gfx/displays/LCDDisplayKompleteKontrol.h
    src/gfx/displays/LCDDisplayKompleteKontrol.cpp
)

set(
  src_gfx_fonts_data_SRCS
    src/gfx/fonts/data/FONT_16-seg.h
    src/gfx/fonts/data/FONT_7-seg.h
    src/gfx/fonts/data/FONT_big.h
    src/gfx/fonts/data/FONT_normal.h
    src/gfx/fonts/data/FONT_small.h
)

set(
  src_gfx_fonts_SRCS
    src/gfx/fonts/FontBig.h
    src/gfx/fonts/FontNormal.h
    src/gfx/fonts/FontSmall.h
)

set(
  src_gfx_SRCS
    src/gfx/Canvas.cpp
    src/gfx/FontManager.cpp
    src/gfx/GDisplay.cpp
)

set(
  src_util_SRCS
    src/util/ColorRGB.cpp
    src/util/Functions.cpp
    src/util/Version.cpp
)

source_group("src\\client"           FILES ${src_client_SRCS})
source_group("src\\comm"             FILES ${src_comm_SRCS})

source_group("src\\comm\\drivers\\HIDAPI"    FILES ${src_comm_drivers_HIDAPI_SRCS})
source_group("src\\comm\\drivers\\LibUSB"    FILES ${src_comm_drivers_LibUSB_SRCS})
source_group("src\\comm\\drivers\\MIDI"      FILES ${src_comm_drivers_MIDI_SRCS})
source_group("src\\comm\\drivers\\Probe"     FILES ${src_comm_drivers_Probe_SRCS})

source_group("src\\devices"          FILES ${src_devices_SRCS})
source_group("src\\devices\\ableton" FILES ${src_devices_ableton_SRCS})
source_group("src\\devices\\akai"    FILES ${src_devices_akai_SRCS})
source_group("src\\devices\\generic" FILES ${src_devices_generic_SRCS})
source_group("src\\devices\\ni"      FILES ${src_devices_ni_SRCS})

source_group("src\\gfx"              FILES ${src_gfx_SRCS})
source_group("src\\gfx\\displays"    FILES ${src_gfx_displays_SRCS})
source_group("src\\gfx\\fonts"       FILES ${src_gfx_fonts_SRCS})
source_group("src\\gfx\\fonts\\data" FILES ${src_gfx_fonts_data_SRCS})

source_group("src\\util"             FILES ${src_util_SRCS})

set(
  cabl_SRCS
    ${src_SRCS}
    ${src_client_SRCS}
    ${src_comm_drivers_HIDAPI_SRCS}
    ${src_comm_drivers_LibUSB_SRCS}
    ${src_comm_drivers_MIDI_SRCS}
    ${src_comm_drivers_Probe_SRCS}
    ${src_comm_SRCS}
    ${src_devices_SRCS}
    ${src_devices_ableton_SRCS}
    ${src_devices_akai_SRCS}
    ${src_devices_generic_SRCS}
    ${src_devices_ni_SRCS}
    ${src_gfx_displays_SRCS}
    ${src_gfx_fonts_SRCS}
    ${src_gfx_fonts_data_SRCS}
    ${src_gfx_SRCS}
    ${src_util_SRCS}
)


# All platforms ---------------------------------------------------------------------------------- #

include_directories(inc src ${RTMIDI_INCLUDE_DIRS} ${LIBUSB_INCLUDE_DIRS} ${HIDAPI_INCLUDE_DIRS} ${UNMIDIFY_INCLUDE_DIRS})
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories( /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/)
endif()

set(
  cabl_ALL
    ${cabl_INCLUDES}
    ${cabl_SRCS}
)

if(BUILD_SHARED_LIBS AND NOT COVERALLS)
  message(STATUS "BUILD_SHARED_LIBS is ON")
  add_library( cabl SHARED ${cabl_ALL} )
  set_target_properties(
    cabl
    PROPERTIES
      VERSION             "${CABL_VERSION_STRING}"
      SOVERSION           "${CABL_VERSION_MAJOR}.${CABL_VERSION_MINOR}"
      OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
      OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
  )

  if(LIBUSB_LOCAL_BUILD)
    target_link_libraries( cabl PUBLIC libusb)
  else()
    target_link_libraries( cabl PUBLIC ${LIBUSB_LIBRARIES})
  endif()

  if(HIDAPI_LOCAL_BUILD)
    target_link_libraries( cabl PUBLIC hidapi)
  else()
    target_link_libraries( cabl PUBLIC ${HIDAPI_LIBRARIES})
  endif()

  if(RTMIDI_LOCAL_BUILD)
    target_link_libraries( cabl PUBLIC rtmidi)
  else()
    target_link_libraries( cabl PUBLIC ${RTMIDI_LIBRARIES})
  endif()

endif()

if(BUILD_STATIC_LIBS)
  message(STATUS "BUILD_STATIC_LIBS is ON")
  add_library( cabl-static STATIC ${cabl_ALL} )
  set_target_properties(
    cabl-static
    PROPERTIES
      VERSION             "${CABL_VERSION_STRING}"
      OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
      OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
  )

  if(LIBUSB_LOCAL_BUILD)
    target_link_libraries( cabl-static PUBLIC libusb)
  else()
    target_link_libraries( cabl-static PUBLIC ${LIBUSB_LIBRARIES})
  endif()

  if(HIDAPI_LOCAL_BUILD)
    target_link_libraries( cabl-static PUBLIC hidapi)
  else()
    target_link_libraries( cabl-static PUBLIC ${HIDAPI_LIBRARIES})
  endif()

  if(RTMIDI_LOCAL_BUILD)
    target_link_libraries( cabl-static PUBLIC rtmidi)
  else()
    target_link_libraries( cabl-static PUBLIC ${RTMIDI_LIBRARIES})
  endif()

endif()

if(CABL_PYTHON AND NOT COVERALLS)
  set(
    cabl_python_SRCS
      src/py/pyCabl.cpp
      src/py/PyClient.cpp
      src/py/PyClient.h
  )

  source_group("py"              FILES ${cabl_python_SRCS})

  if(NOT Boost_PYTHON_FOUND)
    message(WARNING "Boost.python has not been found, the python wrapper will not be created.")
  else()
    add_library( pycabl SHARED ${cabl_ALL} ${cabl_python_SRCS})
    set_target_properties(
      pycabl
      PROPERTIES
        VERSION             "${CABL_VERSION_STRING}"
        SOVERSION           "${CABL_VERSION_MAJOR}.${CABL_VERSION_MINOR}"
        OUTPUT_NAME         "py${LIBRARY_FILE_NAME}"
        SUFFIX              ".so"
        PREFIX              ""
    )

    if(LIBUSB_LOCAL_BUILD)
      target_link_libraries( pycabl PUBLIC libusb)
    else()
      target_link_libraries( pycabl PUBLIC ${LIBUSB_LIBRARIES})
    endif()

    if(HIDAPI_LOCAL_BUILD)
      target_link_libraries( pycabl PUBLIC hidapi)
    else()
      target_link_libraries( pycabl PUBLIC ${HIDAPI_LIBRARIES})
    endif()

    if(RTMIDI_LOCAL_BUILD)
      target_link_libraries( pycabl PUBLIC rtmidi)
    else()
      target_link_libraries( pycabl PUBLIC ${RTMIDI_LIBRARIES})
    endif()

    target_link_libraries( pycabl PUBLIC ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})
    target_include_directories(pycabl PUBLIC ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
  endif()
endif()

# OS X and Linux --------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(BUILD_SHARED_LIBS AND NOT COVERALLS)
    target_link_libraries( cabl PUBLIC pthread)
  endif()

  if(BUILD_STATIC_LIBS)
    target_link_libraries( cabl-static PUBLIC pthread)
  endif()
endif()

if( UNIX )

  if(${IS_CABL})

    if(BUILD_SHARED_LIBS AND NOT COVERALLS)
      install (TARGETS    cabl             DESTINATION lib)
    endif()

    if(BUILD_STATIC_LIBS)
      install (TARGETS    cabl-static      DESTINATION lib)
    endif()

    install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/client       DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

    install (
      FILES        ${PROJECT_BINARY_DIR}/cabl-config.h
      DESTINATION  "include/${LIBRARY_FOLDER_NAME}"
    )
  endif()

endif()


# Windows specific ------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

  if(BUILD_SHARED_LIBS)
    target_link_libraries( cabl  PUBLIC  setupapi.lib)
  endif(BUILD_SHARED_LIBS)

  if(BUILD_STATIC_LIBS)
    target_link_libraries( cabl-static PUBLIC setupapi.lib)
  endif(BUILD_STATIC_LIBS)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


# Arduino/Teensy specific ------------------------------------------------------------------------ #

if(${IS_CABL} AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_custom_target( CreateArduinoLibrary
  command ${CMAKE_SOURCE_DIR}/arduinify.sh ${CMAKE_SOURCE_DIR} artifacts/arduino
    && cp ${PROJECT_BINARY_DIR}/cabl-config.h
          ${CMAKE_SOURCE_DIR}/artifacts/arduino/cabl/cabl-config.h
    && cp ${PROJECT_BINARY_DIR}/library.properties
          ${CMAKE_SOURCE_DIR}/artifacts/arduino/cabl/library.properties
)
configure_file (
  "${PROJECT_SOURCE_DIR}/support/arduino/library.properties.in"
  "${PROJECT_BINARY_DIR}/library.properties"
)
endif()

# Unit tests ------------------------------------------------------------------------------------- #

if(${CABL_TEST})
  addCatch()
  enable_testing(true)
  add_subdirectory(test)
endif()

# Coveralls -------------------------------------------------------------------------------------- #

if(COVERALLS)
  message(STATUS "Coverage build enabled")
  addCoverallsCMake()
  list(APPEND CMAKE_MODULE_PATH "${COVERALLS_CMAKE_PATH}")
  include(Coveralls)
  coveralls_turn_on_coverage()
  coveralls_setup(
    "${cabl_ALL}"
    ${COVERALLS_UPLOAD}
    "${COVERALLS_CMAKE_PATH}"
  )
endif()

# Documentation (doxygen) ------------------------------------------------------------------------ #

if(${CABL_DOCS})

  if(NOT DOXYGEN_FOUND)

    message(FATAL_ERROR "Doxygen is needed to build the documentation.")

  else()

    set( doxyfile_in          ${CABL_ROOT_DIR}/support/doxygen/Doxyfile.in )
    set( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile                          )

    set( doxy_project_name    ${PROJECT_NAME}                                         )
    set( doxy_input_folder    ${PROJECT_SOURCE_DIR}/inc                               )
    set( doxy_output_root     ${PROJECT_SOURCE_DIR}/docs                              )
    set( doxy_readme_md       ${PROJECT_SOURCE_DIR}/README.md                         )

    set( doxy_html_root       ${PROJECT_SOURCE_DIR}/support/doxygen                   )

    set( doxy_project_version "v. ${CABL_VERSION_MAJOR}.${CABL_VERSION_MINOR}.${CABL_VERSION_MICRO}")

    configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

    add_custom_target(
      doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

  endif()
endif()

# Examples --------------------------------------------------------------------------------------- #
if(CABL_EXAMPLES AND NOT COVERALLS)
  add_subdirectory(examples)
endif()

# Clang-tidy and Clang-format -------------------------------------------------------------------- #

include(clang-tools)
clangTidyTarget(cabl-static)
