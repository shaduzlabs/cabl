
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 2.8.0)

project (k-IO)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
include(sl-main)

set(LIBRARY_FILE_NAME     "k-io")
set(LIBRARY_FOLDER_NAME   "k-io")
set(DEBUG_SUFFIX          "_d")

# The version number.
set (k-IO_VERSION_MAJOR 0)
set (k-IO_VERSION_MINOR 9)
set (k-IO_VERSION_MICRO 2)
set( k-IO_VERSION_STRING ${k-IO_VERSION_MAJOR}.${k-IO_VERSION_MINOR}.${k-IO_VERSION_MICRO})

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  set(IS_K-IO TRUE)
else()
  set(IS_K-IO FALSE)
endif()

option(K-IO_TEST      "Build k-IO tests"     ${IS_K-IO})
option(K-IO_DOCS      "Build k-IO docs"      ${IS_K-IO})
option(K-IO_EXAMPLES  "Build k-IO examples"  ${IS_K-IO})

configure_file (
  "${PROJECT_SOURCE_DIR}/inc/k-io-config.h.in"
  "${PROJECT_BINARY_DIR}/k-io-config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# OSX specific ----------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


if(NOT ${IS_K-IO} OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#  addHIDAPI()
#  addLibUSB()
  addRtMidi()
else()
  include(FindHIDAPI)
  include(FindLibUSB)
  include(FindRtMidi)
endif()

addUnmidify()

# Includes --------------------------------------------------------------------------------------- #

set(
  inc_app_INCLUDES
    inc/app/Application.h
)

set(
  inc_comm_INCLUDES
    inc/comm/Driver.h
    inc/comm/DeviceDescriptor.h
    inc/comm/DeviceHandle.h
    inc/comm/Transfer.h
)

set(
  inc_devices_INCLUDES
    inc/devices/Device.h
    inc/devices/DeviceFactory.h
)

set(
  inc_devices_ableton_INCLUDES
    inc/devices/ableton/Push.h
)

set(
  inc_devices_generic_INCLUDES
    inc/devices/generic/USBMidi.h
)

set(
  inc_devices_ni_INCLUDES
    inc/devices/ni/KompleteKontrol.h
    inc/devices/ni/MaschineMK1.h
    inc/devices/ni/MaschineMK2.h
    inc/devices/ni/MaschineMikroMK1.h
    inc/devices/ni/MaschineMikroMK2.h
    inc/devices/ni/TraktorF1MK2.h
)

set(
  inc_gfx_displays_INCLUDES
    inc/gfx/displays/GDisplayDummy.h
    inc/gfx/displays/GDisplayMaschineMikro.h
    inc/gfx/displays/GDisplayMaschineMK1.h
    inc/gfx/displays/GDisplayMaschineMK2.h
    inc/gfx/displays/LCDDisplay7Segments.h
    inc/gfx/displays/LCDDisplayGeneric.h
    inc/gfx/displays/LCDDisplayKompleteKontrol.h
)

set(
  inc_gfx_INCLUDES
    inc/gfx/Canvas.h
    inc/gfx/Font.h
    inc/gfx/GDisplay.h
    inc/gfx/LCDDisplay.h
)

set(
  inc_util_INCLUDES
    inc/util/CircularBuffer.h
    inc/util/Functions.h
    inc/util/LedColor.h
    inc/util/Log.h
    inc/util/Macros.h
    inc/util/RGBColor.h
    inc/util/Types.h
    inc/util/Version.h
)

set(
  inc_INCLUDES
    inc/k-io.h
)

source_group("inc\\app"              FILES ${inc_app_INCLUDES})
source_group("inc\\comm"             FILES ${inc_comm_INCLUDES})
source_group("inc\\devices"          FILES ${inc_devices_INCLUDES})
source_group("inc\\devices\\ableton" FILES ${inc_devices_ableton_INCLUDES})
source_group("inc\\devices\\generic" FILES ${inc_devices_generic_INCLUDES})
source_group("inc\\devices\\ni"      FILES ${inc_devices_ni_INCLUDES})
source_group("inc\\gfx"              FILES ${inc_gfx_INCLUDES})
source_group("inc\\gfx\\displays"    FILES ${inc_gfx_displays_INCLUDES})
source_group("inc\\util"             FILES ${inc_util_INCLUDES})
source_group("inc"                   FILES ${inc_INCLUDES})

set (
  k-IO_INCLUDES
    ${inc_app_INCLUDES}
    ${inc_comm_INCLUDES}
    ${inc_devices_INCLUDES}
    ${inc_devices_ableton_INCLUDES}
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
  src_app_SRCS
    src/app/Application.cpp
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
    src/comm/Driver.cpp
    src/comm/DriverImpl.h
    src/comm/DeviceHandle.cpp
    src/comm/DeviceHandleImpl.h
    src/comm/Transfer.cpp
)

set(
  src_devices_SRCS
    src/devices/Device.cpp
    src/devices/DeviceFactory.cpp
)

set(
  src_devices_ableton_SRCS
    src/devices/ableton/Push.cpp
)

set(
  src_devices_generic_SRCS
    src/devices/generic/USBMidi.cpp
)

set(
  src_devices_ni_SRCS
    src/devices/ni/KompleteKontrol.cpp
    src/devices/ni/MaschineMK1.cpp
    src/devices/ni/MaschineMK2.cpp
    src/devices/ni/MaschineMikroMK2.cpp
    src/devices/ni/TraktorF1MK2.cpp
)

set(
  src_gfx_displays_SRCS
    src/gfx/displays/GDisplayMaschineMikro.cpp
    src/gfx/displays/GDisplayMaschineMK1.cpp
    src/gfx/displays/GDisplayMaschineMK2.cpp
    src/gfx/displays/LCDDisplay7Segments.cpp
    src/gfx/displays/LCDDisplayGeneric.cpp
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
    src/gfx/GDisplay.cpp
)

set(
  src_util_SRCS
    src/util/CircularBuffer.cpp
    src/util/Functions.cpp
    src/util/Version.cpp
)

set(
  src_SRCS
    src/k-io.cpp
)

source_group("src\\app"              FILES ${src_app_SRCS})
source_group("src\\comm"             FILES ${src_comm_SRCS})
#source_group("src\\comm\\drivers")

source_group("src\\comm\\drivers\\HIDAPI"    FILES ${src_comm_drivers_HIDAPI_SRCS})
source_group("src\\comm\\drivers\\LibUSB"    FILES ${src_comm_drivers_LibUSB_SRCS})
source_group("src\\comm\\drivers\\MIDI"      FILES ${src_comm_drivers_MIDI_SRCS})
source_group("src\\comm\\drivers\\Probe"     FILES ${src_comm_drivers_Probe_SRCS})

source_group("src\\devices"          FILES ${src_devices_SRCS})
source_group("src\\devices\\ableton" FILES ${src_devices_ableton_SRCS})
source_group("src\\devices\\generic" FILES ${src_devices_generic_SRCS})
source_group("src\\devices\\ni"      FILES ${src_devices_ni_SRCS})

source_group("src\\gfx"              FILES ${src_gfx_SRCS})
source_group("src\\gfx\\displays"    FILES ${src_gfx_displays_SRCS})
source_group("src\\gfx\\fonts"       FILES ${src_gfx_fonts_SRCS})
source_group("src\\gfx\\fonts\\data" FILES ${src_gfx_fonts_data_SRCS})

source_group("src\\util"             FILES ${src_util_SRCS})

source_group("src"                   FILES ${src_SRCS})

set(
  k-IO_SRCS
    ${src_SRCS}
    ${src_app_SRCS}
    ${src_comm_drivers_HIDAPI_SRCS}
    ${src_comm_drivers_LibUSB_SRCS}
    ${src_comm_drivers_MIDI_SRCS}
    ${src_comm_drivers_Probe_SRCS}
    ${src_comm_SRCS}
    ${src_devices_SRCS}
    ${src_devices_ableton_SRCS}
    ${src_devices_generic_SRCS}
    ${src_devices_ni_SRCS}
    ${src_gfx_displays_SRCS}
    ${src_gfx_fonts_SRCS}
    ${src_gfx_fonts_data_SRCS}
    ${src_gfx_SRCS}
    ${src_util_SRCS}
)


# All platforms ---------------------------------------------------------------------------------- #

include_directories(inc src submodules/unmidify/include ${RTMIDI_INCLUDE_DIR})
include_directories(${LIBUSB_INCLUDE_DIRS} ${HIDAPI_INCLUDE_DIR} ${UNMIDIFY_INCLUDE_DIR})

set(
  k-IO_ALL
    ${k-IO_INCLUDES}
    ${k-IO_SRCS}
)

add_library( k-IO SHARED ${k-IO_ALL} )
set_target_properties(
  k-IO
  PROPERTIES
    VERSION             "${k-IO_VERSION_STRING}"
    SOVERSION           "${k-IO_VERSION_STRING}"
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

add_library( k-IOStatic STATIC ${k-IO_ALL} )
set_target_properties(
  k-IOStatic
  PROPERTIES
    VERSION             "${k-IO_VERSION_STRING}"
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

# OS X and Linux --------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries( k-IO PUBLIC usb hidapi-libusb rtmidi pthread)
  target_link_libraries( k-IOStatic PUBLIC hidapi-libusb usb-1.0 rtmidi pthread)
  target_include_directories( k-IO PRIVATE /usr/include/hidapi /usr/include/libusb-1.0)
  target_include_directories( k-IOStatic PRIVATE /usr/include/hidapi /usr/include/libusb-1.0)
endif()

if(APPLE)
  target_link_libraries( k-IO PUBLIC libusb hidapi rtmidi)
  target_link_libraries( k-IOStatic PUBLIC libusb hidapi rtmidi)
endif()

if(APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  if(${IS_K-IO})

    install (TARGETS    k-IO             DESTINATION lib)
    install (TARGETS    k-IOStatic       DESTINATION lib)
    install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/app          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
    install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

    install (
      FILES        ${PROJECT_BINARY_DIR}/k-io-config.h
      DESTINATION  "include/${LIBRARY_FOLDER_NAME}"
    )
  endif()

endif()


# Windows specific ------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

  set_target_properties(
    k-IO
    PROPERTIES
      EXCLUDE_FROM_ALL            TRUE
      EXCLUDE_FROM_DEFAULT_BUILD  TRUE
  )
  target_link_libraries( k-IO        rtmidi libusb hidapi setupapi.lib)
  target_link_libraries( k-IOStatic  rtmidi libusb hidapi setupapi.lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


# Arduino/Teensy specific ------------------------------------------------------------------------ #

if(${IS_K-IO} AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_custom_target( CreateArduinoLibrary
  command ${CMAKE_SOURCE_DIR}/arduinify.sh ${CMAKE_SOURCE_DIR} artifacts/arduino &&
     cp ${PROJECT_BINARY_DIR}/k-io-config.h ${CMAKE_SOURCE_DIR}/artifacts/arduino/k-IO/k-io-config.h
)
endif()

# Unit tests ------------------------------------------------------------------------------------- #
if(${K-IO_TEST})
  addCatch()
  add_subdirectory(test)
endif()

# Documentation (doxygen) ------------------------------------------------------------------------ #

if(${K-IO_DOCS})
  find_package(Doxygen)

  if(NOT DOXYGEN_FOUND)

    message(FATAL_ERROR "Doxygen is needed to build the documentation.")

  else()

    set( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/support/doxygen/Doxyfile.in )
    set( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile                          )

    set( doxy_project_name    ${PROJECT_NAME}                                         )
    set( doxy_input_folder    ${PROJECT_SOURCE_DIR}/inc                               )
    set( doxy_output_root     ${PROJECT_SOURCE_DIR}/docs                              )
    set( doxy_readme_md       ${PROJECT_SOURCE_DIR}/README.md                         )

    set( doxy_html_root       ${PROJECT_SOURCE_DIR}/support/doxygen                   )

    set( doxy_project_version "v. ${k-IO_VERSION_MAJOR}.${k-IO_VERSION_MINOR}.${k-IO_VERSION_MICRO}")

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
if(K-IO_EXAMPLES)
  add_subdirectory(examples)
endif()
