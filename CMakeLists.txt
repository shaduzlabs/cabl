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

project (kompletIO)

set(LIBRARY_FILE_NAME     "k-io")
set(LIBRARY_FOLDER_NAME   "k-io")
set(DEBUG_SUFFIX          "_d")

# The version number.
set (kompletIO_VERSION_MAJOR 0)
set (kompletIO_VERSION_MINOR 2)
set (kompletIO_VERSION_MICRO 1)
 
configure_file (
  "${PROJECT_SOURCE_DIR}/inc/k-io-config.h.in"
  "${PROJECT_BINARY_DIR}/k-io-config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# C++14: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

# Libraries ---------------------------------------------------------------------------------------------------------- #

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  find_package(hidapi REQUIRED)
  find_package(libusb REQUIRED)
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
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

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Includes ----------------------------------------------------------------------------------------------------------- #

SET( 
  inc_comm_INCLUDES          
    inc/comm/Driver.h
    inc/comm/DeviceDescriptor.h
    inc/comm/DeviceHandle.h
    inc/comm/Transfer.h
)

SET( 
  inc_devices_INCLUDES       
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
    inc/util/CircularBuffer_SL.h
    inc/util/Functions_SL.h
    inc/util/Version_SL.h
)

SET( 
  inc_INCLUDES
    inc/Application.h
    inc/Device.h
    inc/k-io.h
    inc/Macros.h
    inc/Types.h
)

source_group("inc\\comm"             FILES ${inc_comm_INCLUDES})
source_group("inc\\devices"          FILES ${inc_devices_INCLUDES})
source_group("inc\\gfx"              FILES ${inc_gfx_INCLUDES})
source_group("inc\\gfx\\displays"    FILES ${inc_gfx_displays_INCLUDES})
source_group("inc\\util"             FILES ${inc_util_INCLUDES})
source_group("inc"                   FILES ${inc_INCLUDES})

SET ( 
  kompletIO_INCLUDES
    ${inc_comm_INCLUDES}
    ${inc_devices_INCLUDES}
    ${inc_gfx_displays_INCLUDES}
    ${inc_gfx_INCLUDES}
    ${inc_util_INCLUDES}
    ${inc_INCLUDES}
)


# Source files ------------------------------------------------------------------------------------------------------- #

SET( 
  src_comm_drivers_SRCS      
    src/comm/drivers/DriverHIDAPI.cpp
    src/comm/drivers/DriverHIDAPI.h
    src/comm/drivers/DriverLIBUSB.cpp
    src/comm/drivers/DriverLIBUSB.h
    src/comm/drivers/DriverMOCK.cpp
    src/comm/drivers/DriverMOCK.h
    src/comm/drivers/DriverMAX3421E.cpp
    src/comm/drivers/DriverMAX3421E.h
    src/comm/drivers/DriverSAM3X8E.cpp
    src/comm/drivers/DriverSAM3X8E.h
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
    src/devices/DeviceMaschineMK1.cpp
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
    src/util/CircularBuffer_SL.cpp
    src/util/Functions_SL.cpp
    src/util/Version_SL.cpp
)

SET( 
  src_SRCS
    src/Application.cpp
    src/Device.cpp
    src/k-io.cpp
)

source_group("src\\comm"             FILES ${src_comm_SRCS})
source_group("src\\comm\\drivers"    FILES ${src_comm_drivers_SRCS})
source_group("src\\devices"          FILES ${src_devices_SRCS})
source_group("src\\gfx"              FILES ${src_gfx_SRCS})
source_group("src\\gfx\\displays"    FILES ${src_gfx_displays_SRCS})
source_group("src\\gfx\\fonts"       FILES ${src_gfx_fonts_SRCS})
source_group("src\\gfx\\fonts\\data" FILES ${src_gfx_fonts_data_SRCS})
source_group("src\\util"             FILES ${src_util_SRCS})
source_group("src"                   FILES ${src_SRCS})

SET ( 
      kompletIO_SRCS
      ${src_SRCS}
      ${src_comm_drivers_SRCS}
      ${src_comm_SRCS}
      ${src_devices_SRCS}
      ${src_gfx_displays_SRCS}
      ${src_gfx_fonts_SRCS}
      ${src_gfx_fonts_data_SRCS}
      ${src_gfx_SRCS}
      ${src_util_SRCS}
)


# All platforms ------------------------------------------------------------------------------------------------------ #

include_directories(inc src ${LibUSB_INCLUDE_DIRS} ${HIDAPI_INCLUDE_DIR})

SET ( 
  kompletIO_ALL
    ${kompletIO_INCLUDES}
    ${kompletIO_SRCS}
)

add_library( kompletIO SHARED ${kompletIO_ALL} )
set_target_properties(
  kompletIO
  PROPERTIES 
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

add_library( kompletIOStatic STATIC ${kompletIO_ALL} )
set_target_properties(
  kompletIOStatic 
  PROPERTIES 
    OUTPUT_NAME         "${LIBRARY_FILE_NAME}"
    OUTPUT_NAME_DEBUG   "${LIBRARY_FILE_NAME}${DEBUG_SUFFIX}"
)

# OS X specific ------------------------------------------------------------------------------------------------------ #

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  target_link_libraries( kompletIO PUBLIC ${LibUSB_LIBRARIES} ${HIDAPI_LIBRARY})

  install (TARGETS    kompletIO        DESTINATION lib)
  install (TARGETS    kompletIOStatic  DESTINATION lib)
  install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


# Linux specific ----------------------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  target_link_libraries( kompletIO ${LibUSB_LIBRARIES} ${HIDAPI_LIBRARY} pthread)

  install (TARGETS    kompletIO        DESTINATION lib)
  install (TARGETS    kompletIOStatic  DESTINATION lib)
  install (FILES      ${inc_INCLUDES}  DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/comm         DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/devices      DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/gfx          DESTINATION "include/${LIBRARY_FOLDER_NAME}")
  install (DIRECTORY  inc/util         DESTINATION "include/${LIBRARY_FOLDER_NAME}")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# Windows specific --------------------------------------------------------------------------------------------------- #

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  
  target_link_libraries( kompletIO        libusb hidapi setupapi.lib)
  target_link_libraries( kompletIOStatic  libusb hidapi setupapi.lib)
  
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Arduino/Teensy specific -------------------------------------------------------------------------------------------- #


# -------------------------------------------------------------------------------------------------------------------- #
#  Examples                                                                                                            #
# -------------------------------------------------------------------------------------------------------------------- #

add_subdirectory(examples Examples)


