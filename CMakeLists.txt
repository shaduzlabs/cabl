# -------------------------------------------------------------------------------------------------------------------- #

                 #################
                 #################
                 ###           ###
                 ###           ###
                 ###           ###
####################           #########################################################################################
####################           ################################################################ www.shaduzlabs.com #####

# -------------------------------------------------------------------------------------------------------------------- #
#  k-IO Libraries                                                                                                      #
# -------------------------------------------------------------------------------------------------------------------- #

cmake_minimum_required (VERSION 2.8.0)

project (k-IO)

set(LIBRARY_FILE_NAME     "k-io")
set(LIBRARY_FOLDER_NAME   "k-io")
set(DEBUG_SUFFIX          "_d")

# The version number.
set (k-IO_VERSION_MAJOR 0)
set (k-IO_VERSION_MINOR 2)
set (k-IO_VERSION_MICRO 1)
 
configure_file (
  "${PROJECT_SOURCE_DIR}/inc/k-io-config.h.in"
  "${PROJECT_BINARY_DIR}/k-io-config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# C++14: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

# Libraries ---------------------------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  find_package(hidapi REQUIRED)
  find_package(libusb REQUIRED)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

  set(HIDAPI_INCLUDE_DIR "submodules/hidapi/hidapi/")
  set(LibUSB_INCLUDE_DIRS "submodules/libusb/libusb/" "submodules/libusb/libusb/os/" )

  SET( 
    lib_hidapi_LIBRARY      
      submodules/hidapi/hidapi/hidapi.h
      submodules/hidapi/windows/hid.c
  )
  
  SET( 
    lib_libusb_LIBRARY      
      submodules/libusb/msvc/config.h
      submodules/libusb/libusb/core.c
      submodules/libusb/libusb/descriptor.c
      submodules/libusb/libusb/hotplug.c
      submodules/libusb/libusb/io.c
      submodules/libusb/libusb/os/poll_windows.c
      submodules/libusb/libusb/strerror.c
      submodules/libusb/libusb/sync.c
      submodules/libusb/libusb/os/threads_windows.c
      submodules/libusb/libusb/os/windows_usb.c
  )

  source_group("src"           FILES ${lib_hidapi_LIBRARY})
  source_group("src"           FILES ${lib_libusb_LIBRARY})

  add_library( hidapi STATIC ${lib_hidapi_LIBRARY} )
  set_target_properties(
  hidapi 
  PROPERTIES 
    OUTPUT_NAME         "hidapi"
    OUTPUT_NAME_DEBUG   "hidapi${DEBUG_SUFFIX}"
  )

  add_library( libusb STATIC ${lib_libusb_LIBRARY} )
  set_target_properties(
  libusb 
  PROPERTIES 
    OUTPUT_NAME         "libusb"
    OUTPUT_NAME_DEBUG   "libusb${DEBUG_SUFFIX}"
  )
  target_include_directories( libusb PRIVATE "submodules/libusb/msvc/" )

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

 set(HIDAPI_INCLUDE_DIR "submodules/hidapi/hidapi/")
 set(LibUSB_INCLUDE_DIRS "submodules/libusb/libusb/" "submodules/libusb/libusb/os/" )

  SET( 
    lib_hidapi_LIBRARY      
      submodules/hidapi/hidapi/hidapi.h
      submodules/hidapi/mac/hid.c
  )
  
  SET( 
    lib_libusb_LIBRARY      
      submodules/libusb/Xcode/config.h
      submodules/libusb/libusb/core.c
      submodules/libusb/libusb/descriptor.c
      submodules/libusb/libusb/hotplug.c
      submodules/libusb/libusb/io.c
      submodules/libusb/libusb/os/poll_posix.c
      submodules/libusb/libusb/strerror.c
      submodules/libusb/libusb/sync.c
      submodules/libusb/libusb/os/threads_posix.c
      submodules/libusb/libusb/os/darwin_usb.c
  )

  source_group("src"           FILES ${lib_hidapi_LIBRARY})
  source_group("src"           FILES ${lib_libusb_LIBRARY})

  add_library( hidapi STATIC ${lib_hidapi_LIBRARY} )
  set_target_properties(
  hidapi 
  PROPERTIES 
    OUTPUT_NAME         "hidapi"
    OUTPUT_NAME_DEBUG   "hidapi${DEBUG_SUFFIX}"
  )

  add_library( libusb STATIC ${lib_libusb_LIBRARY} )
  set_target_properties(
  libusb 
  PROPERTIES 
    OUTPUT_NAME         "libusb"
    OUTPUT_NAME_DEBUG   "libusb${DEBUG_SUFFIX}"
  )
  target_include_directories( libusb PRIVATE "submodules/libusb/Xcode/" )

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# RtMidi ------------------------------------------------------------------------------------------------------------- #
set(RTMIDI_INCLUDE_DIR "submodules/rtmidi/")
SET( 
  lib_rtmidi_LIBRARY      
    submodules/rtmidi/RtMidi.h
    submodules/rtmidi/RtMidi.cpp
)
source_group("src"  FILES  ${lib_rtmidi_LIBRARY})
add_library( rtmidi STATIC ${lib_rtmidi_LIBRARY})

set_target_properties(
  rtmidi 
  PROPERTIES 
    OUTPUT_NAME         "rtmidi"
    OUTPUT_NAME_DEBUG   "rtmidi${DEBUG_SUFFIX}"
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set_target_properties(
  rtmidi 
  PROPERTIES 
    COMPILE_DEFINITIONS __MACOSX_CORE__
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Includes ----------------------------------------------------------------------------------------------------------- #

SET( 
  inc_app_INCLUDES          
    inc/app/Application.h
)

SET( 
  inc_comm_INCLUDES          
    inc/comm/Driver.h
    inc/comm/DeviceDescriptor.h
    inc/comm/DeviceHandle.h
    inc/comm/Transfer.h
)

SET( 
  inc_devices_INCLUDES       
    inc/devices/Device.h
    inc/devices/DeviceFactory.h
    inc/devices/DeviceMaschineMK1.h
    inc/devices/DeviceMaschineMK2.h
    inc/devices/DeviceMaschineMikroMK1.h
    inc/devices/DeviceMaschineMikroMK2.h
)

SET( 
  inc_gfx_displays_INCLUDES  
    inc/gfx/displays/GDisplayMaschineMikro.h
    inc/gfx/displays/GDisplayMaschineMK1.h
    inc/gfx/displays/GDisplayMaschineMK2.h
)

SET( 
  inc_gfx_INCLUDES
    inc/gfx/Canvas.h
    inc/gfx/Font.h
    inc/gfx/GDisplay.h
)

SET( 
  inc_util_INCLUDES
    inc/util/CircularBuffer.h
    inc/util/Functions.h
    inc/util/Macros.h
    inc/util/MidiMessage.h
    inc/util/Types.h
    inc/util/Version.h
)

SET( 
  inc_INCLUDES
    inc/k-io.h
)

source_group("inc\\app"              FILES ${inc_app_INCLUDES})
source_group("inc\\comm"             FILES ${inc_comm_INCLUDES})
source_group("inc\\devices"          FILES ${inc_devices_INCLUDES})
source_group("inc\\gfx"              FILES ${inc_gfx_INCLUDES})
source_group("inc\\gfx\\displays"    FILES ${inc_gfx_displays_INCLUDES})
source_group("inc\\util"             FILES ${inc_util_INCLUDES})
source_group("inc"                   FILES ${inc_INCLUDES})

SET ( 
  k-IO_INCLUDES
    ${inc_app_INCLUDES}
    ${inc_comm_INCLUDES}
    ${inc_devices_INCLUDES}
    ${inc_gfx_displays_INCLUDES}
    ${inc_gfx_INCLUDES}
    ${inc_util_INCLUDES}
    ${inc_INCLUDES}
)


# Source files ------------------------------------------------------------------------------------------------------- #

SET( 
  src_app_SRCS
    src/app/Application.cpp
)

SET( 
  src_comm_drivers_HIDAPI_SRCS      
    src/comm/drivers/HIDAPI/DriverHIDAPI.cpp
    src/comm/drivers/HIDAPI/DriverHIDAPI.h
    src/comm/drivers/HIDAPI/DeviceHandleHIDAPI.cpp
    src/comm/drivers/HIDAPI/DeviceHandleHIDAPI.h
)

SET( 
  src_comm_drivers_LibUSB_SRCS      
    src/comm/drivers/LibUSB/DriverLibUSB.cpp
    src/comm/drivers/LibUSB/DriverLibUSB.h
    src/comm/drivers/LibUSB/DeviceHandleLibUSB.cpp
    src/comm/drivers/LibUSB/DeviceHandleLibUSB.h
)

SET( 
  src_comm_drivers_MAX3421E_SRCS      
    src/comm/drivers/MAX3421E/DriverMAX3421E.cpp
    src/comm/drivers/MAX3421E/DriverMAX3421E.h
    src/comm/drivers/MAX3421E/DeviceHandleMAX3421E.cpp
    src/comm/drivers/MAX3421E/DeviceHandleMAX3421E.h
)

SET( 
  src_comm_drivers_Probe_SRCS      
    src/comm/drivers/Probe/DriverProbe.cpp
    src/comm/drivers/Probe/DriverProbe.h
    src/comm/drivers/Probe/DeviceHandleProbe.cpp
    src/comm/drivers/Probe/DeviceHandleProbe.h
)

SET( 
  src_comm_drivers_SAM3X8E_SRCS      
    src/comm/drivers/SAM3X8E/DriverSAM3X8E.cpp
    src/comm/drivers/SAM3X8E/DriverSAM3X8E.h
    src/comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.cpp
    src/comm/drivers/SAM3X8E/DeviceHandleSAM3X8E.h
)

SET( 
  src_comm_SRCS
    src/comm/Driver.cpp
    src/comm/DriverImpl.h
    src/comm/DeviceHandle.cpp
    src/comm/DeviceHandleImpl.h
    src/comm/Transfer.cpp
)

SET( 
  src_devices_SRCS
    src/devices/DeviceFactory.cpp
    src/devices/DeviceMaschineMK1.cpp
    src/devices/DeviceMaschineMK2.cpp
    src/devices/DeviceMaschineMikroMK2.cpp
)

SET( 
  src_gfx_displays_SRCS
    src/gfx/displays/GDisplayMaschineMikro.cpp
    src/gfx/displays/GDisplayMaschineMK1.cpp
    src/gfx/displays/GDisplayMaschineMK2.cpp
)

SET( 
  src_gfx_fonts_data_SRCS
    src/gfx/fonts/data/FONT_big.h
    src/gfx/fonts/data/FONT_normal.h
    src/gfx/fonts/data/FONT_small.h
)

SET( 
  src_gfx_fonts_SRCS
    src/gfx/fonts/FontBig.h
    src/gfx/fonts/FontNormal.h
    src/gfx/fonts/FontSmall.h
)

SET( 
  src_gfx_SRCS
    src/gfx/Canvas.cpp
    src/gfx/GDisplay.cpp
)

SET( 
  src_util_SRCS
    src/util/CircularBuffer.cpp
    src/util/Functions.cpp
    src/util/Version.cpp
)

SET( 
  src_SRCS
    src/k-io.cpp
)

source_group("src\\app"              FILES ${src_app_SRCS})
source_group("src\\comm"             FILES ${src_comm_SRCS})
#source_group("src\\comm\\drivers")

source_group("src\\comm\\drivers\\HIDAPI"    FILES ${src_comm_drivers_HIDAPI_SRCS})
source_group("src\\comm\\drivers\\LibUSB"    FILES ${src_comm_drivers_LibUSB_SRCS})
source_group("src\\comm\\drivers\\MAX3421E"  FILES ${src_comm_drivers_MAX3421E_SRCS})
source_group("src\\comm\\drivers\\Probe"     FILES ${src_comm_drivers_Probe_SRCS})
source_group("src\\comm\\drivers\\SAM3X8E"   FILES ${src_comm_drivers_SAM3X8E_SRCS})

source_group("src\\devices"          FILES ${src_devices_SRCS})
source_group("src\\gfx"              FILES ${src_gfx_SRCS})
source_group("src\\gfx\\displays"    FILES ${src_gfx_displays_SRCS})
source_group("src\\gfx\\fonts"       FILES ${src_gfx_fonts_SRCS})
source_group("src\\gfx\\fonts\\data" FILES ${src_gfx_fonts_data_SRCS})
source_group("src\\util"             FILES ${src_util_SRCS})
source_group("src"                   FILES ${src_SRCS})

SET ( 
      k-IO_SRCS
      ${src_SRCS}
      ${src_app_SRCS}
      ${src_comm_drivers_HIDAPI_SRCS}
      ${src_comm_drivers_LibUSB_SRCS}
      ${src_comm_drivers_MAX3421E_SRCS}
      ${src_comm_drivers_Probe_SRCS}
      ${src_comm_drivers_SAM3X8E_SRCS}
      ${src_comm_SRCS}
      ${src_devices_SRCS}
      ${src_gfx_displays_SRCS}
      ${src_gfx_fonts_SRCS}
      ${src_gfx_fonts_data_SRCS}
      ${src_gfx_SRCS}
      ${src_util_SRCS}
)


# All platforms ------------------------------------------------------------------------------------------------------ #

include_directories(inc src ${LibUSB_INCLUDE_DIRS} ${HIDAPI_INCLUDE_DIR} ${RTMIDI_INCLUDE_DIR})

SET ( 
  k-IO_ALL
    ${k-IO_INCLUDES}
    ${k-IO_SRCS}
)

add_library( k-IO SHARED ${k-IO_ALL} )
set_target_properties(
  k-IO
  PROPERTIES 
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

add_library( k-IOStatic STATIC ${k-IO_ALL} )
set_target_properties(
  k-IOStatic 
  PROPERTIES 
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

# OS X specific ------------------------------------------------------------------------------------------------------ #

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  target_link_libraries( k-IO PUBLIC libusb hidapi "-framework CoreFoundation" "-framework IOKit" objc)
  target_link_libraries( k-IO PUBLIC rtmidi "-framework CoreAudio" "-framework CoreMidi")

  install (TARGETS    k-IO             DESTINATION lib)
  install (TARGETS    k-IOStatic       DESTINATION lib)
  install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/app          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


# Linux specific ----------------------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  target_link_libraries( k-IO ${LibUSB_LIBRARIES} ${HIDAPI_LIBRARY} pthread rtmidi)

  install (TARGETS    k-IO             DESTINATION lib)
  install (TARGETS    k-IOStatic       DESTINATION lib)
  install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/app          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# Windows specific --------------------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  
  target_link_libraries( k-IO        rtmidi libusb hidapi setupapi.lib)
  target_link_libraries( k-IOStatic  rtmidi libusb hidapi setupapi.lib)
  
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Arduino/Teensy specific -------------------------------------------------------------------------------------------- #


# -------------------------------------------------------------------------------------------------------------------- #
#  Examples                                                                                                            #
# -------------------------------------------------------------------------------------------------------------------- #

add_subdirectory(examples Examples)

option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ON)
IF(BUILD_DOCUMENTATION)
 
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR
      "Doxygen is needed to build the documentation.")
  ENDIF()
 
  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/support/docs/Doxyfile.in )
  SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile              )
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
  SET( doxy_output_root     ${PROJECT_SOURCE_DIR}/docs                 ) 
  SET( doxy_input           ${PROJECT_SOURCE_DIR}/inc                   ) 
  SET( doxy_extra_files     ${CMAKE_CURRENT_SOURCE_DIR}/support/docs/main.dox )
  SET( doxy_library_version "v. 1.2.3.4")
 
  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )
 
  add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/adocs )
ENDIF()
