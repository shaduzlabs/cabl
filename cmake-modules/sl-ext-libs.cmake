
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

include(sl-functions)

# ------------------------------------------------------------------------------------------------ #
#  RtMidi                                                                                          #
# ------------------------------------------------------------------------------------------------ #
function (addRtMidi)
  if(DEFINED RTMIDI_INCLUDE_DIRS)
    message(STATUS "RtMidi is already available")
  else()
    checkout_external_project(rtmidi https://github.com/thestk/rtmidi.git master)
    set(RTMIDI_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/rtmidi/src/rtmidi PARENT_SCOPE)
    set(RTMIDI_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/rtmidi/src/rtmidi)
    message(STATUS "RtMidi path: ${RTMIDI_INCLUDE_DIRS}")

    set(
      lib_rtmidi_LIBRARY
        ${RTMIDI_INCLUDE_DIRS}/RtMidi.h
        ${RTMIDI_INCLUDE_DIRS}/RtMidi.cpp
    )
    source_group("src"  FILES  ${lib_rtmidi_LIBRARY})
    add_library( rtmidi STATIC ${lib_rtmidi_LIBRARY})
    if(APPLE)
      target_link_libraries(rtmidi PUBLIC "-framework CoreFoundation" "-framework IOKit")
      target_link_libraries(rtmidi PUBLIC "-framework CoreAudio" "-framework CoreMidi" objc)
    endif()

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
  endif()
endfunction()


# ------------------------------------------------------------------------------------------------ #
#  Catch                                                                                           #
# ------------------------------------------------------------------------------------------------ #
function (addCatch)
  if(DEFINED CATCH_INCLUDE_DIRS)
    message(STATUS "Catch is already available")
  else()
    checkout_external_project(catch https://github.com/philsquared/Catch.git master)
    set(CATCH_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/catch/src/catch/include PARENT_SCOPE)
    set(CATCH_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/catch/src/catch/include)
    message(STATUS "Catch path: ${CATCH_INCLUDE_DIRS}")
  endif()
endfunction()


# ------------------------------------------------------------------------------------------------ #
#  MPL                                                                                             #
# ------------------------------------------------------------------------------------------------ #
function (addMPL)
  if(DEFINED MPL_INCLUDE_DIRS)
    message(STATUS "Catch is already available")
  else()
    checkout_external_project(mpl https://github.com/rabauke/mpl.git master)
    set(MPL_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/mpl/src/mpl/mpl PARENT_SCOPE)
    set(MPL_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/mpl/src/mpl/mpl)
    message(STATUS "MPL path: ${MPL_INCLUDE_DIRS}")
  endif()
endfunction()


# ------------------------------------------------------------------------------------------------ #
#  unmidify                                                                                        #
# ------------------------------------------------------------------------------------------------ #
function (addUnmidify)
  if(DEFINED UNMIDIFY_INCLUDE_DIRS)
    message(STATUS "Unmidify is already available")
  else()
    checkout_external_project(unmidify https://github.com/shaduzlabs/unmidify.git master)
    set(UNMIDIFY_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/unmidify/src/unmidify/include PARENT_SCOPE)
    set(UNMIDIFY_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/unmidify/src/unmidify/include)
    message(STATUS "Unmidify path: ${UNMIDIFY_INCLUDE_DIRS}")
  endif()
endfunction()


# ------------------------------------------------------------------------------------------------ #
#  LibUSB                                                                                          #
# ------------------------------------------------------------------------------------------------ #
function (addLibUSB)
  if(DEFINED LIBUSB_INCLUDE_DIRS)
    message(STATUS "libUSB is already available")
  else()
    checkout_external_project(libusb https://github.com/libusb/libusb.git master)
    set(LIBUSB_BASE_DIR ${CMAKE_BINARY_DIR}/libusb/src/libusb/)
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_BASE_DIR}/libusb ${LIBUSB_BASE_DIR}/libusb/os PARENT_SCOPE)
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_BASE_DIR}/libusb ${LIBUSB_BASE_DIR}/libusb/os)

    message(STATUS "libUSB path: ${LIBUSB_BASE_DIR}")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      set(
        lib_libusb_LIBRARY
          ${LIBUSB_BASE_DIR}/msvc/config.h
          ${LIBUSB_BASE_DIR}/libusb/core.c
          ${LIBUSB_BASE_DIR}/libusb/descriptor.c
          ${LIBUSB_BASE_DIR}/libusb/hotplug.c
          ${LIBUSB_BASE_DIR}/libusb/io.c
          ${LIBUSB_BASE_DIR}/libusb/os/poll_windows.c
          ${LIBUSB_BASE_DIR}/libusb/strerror.c
          ${LIBUSB_BASE_DIR}/libusb/sync.c
          ${LIBUSB_BASE_DIR}/libusb/os/threads_windows.c
          ${LIBUSB_BASE_DIR}/libusb/os/windows_nt_common.c
          ${LIBUSB_BASE_DIR}/libusb/os/windows_usbdk.c
          ${LIBUSB_BASE_DIR}/libusb/os/windows_winusb.c
      )
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(
        lib_libusb_LIBRARY
          ${LIBUSB_BASE_DIR}/msvc/config.h
          ${LIBUSB_BASE_DIR}/libusb/core.c
          ${LIBUSB_BASE_DIR}/libusb/descriptor.c
          ${LIBUSB_BASE_DIR}/libusb/hotplug.c
          ${LIBUSB_BASE_DIR}/libusb/io.c
          ${LIBUSB_BASE_DIR}/libusb/os/poll_posix.c
          ${LIBUSB_BASE_DIR}/libusb/strerror.c
          ${LIBUSB_BASE_DIR}/libusb/sync.c
          ${LIBUSB_BASE_DIR}/libusb/os/threads_posix.c
      )
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(
        lib_libusb_LIBRARY
          ${LIBUSB_BASE_DIR}/msvc/config.h
          ${LIBUSB_BASE_DIR}/libusb/core.c
          ${LIBUSB_BASE_DIR}/libusb/descriptor.c
          ${LIBUSB_BASE_DIR}/libusb/hotplug.c
          ${LIBUSB_BASE_DIR}/libusb/io.c
          ${LIBUSB_BASE_DIR}/libusb/os/poll_posix.c
          ${LIBUSB_BASE_DIR}/libusb/strerror.c
          ${LIBUSB_BASE_DIR}/libusb/sync.c
          ${LIBUSB_BASE_DIR}/libusb/os/threads_posix.c
          ${LIBUSB_BASE_DIR}/libusb/os/darwin_usb.c
      )
    endif()
    source_group("src" FILES  ${lib_libusb_LIBRARY})
    add_library(libusb STATIC ${lib_libusb_LIBRARY})
    set_target_properties(
      libusb
      PROPERTIES
        OUTPUT_NAME         "libusb"
        OUTPUT_NAME_DEBUG   "libusb${DEBUG_SUFFIX}"
    )

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      target_include_directories(libusb PUBLIC ${LIBUSB_INCLUDE_DIRS} ${LIBUSB_BASE_DIR}/msvc)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      target_include_directories(libusb PUBLIC ${LIBUSB_INCLUDE_DIRS} ${LIBUSB_BASE_DIR}/Xcode)
    endif()

  endif()
endfunction()

# ------------------------------------------------------------------------------------------------ #
#  HIDAPI                                                                                          #
# ------------------------------------------------------------------------------------------------ #
function (addHIDAPI)
  if(DEFINED HIDAPI_INCLUDE_DIRS)
    message(STATUS "HIDAPI is already available: ${HIDAPI_INCLUDE_DIRS}")
  else()
    checkout_external_project(hidapi https://github.com/signal11/hidapi.git master)
    set(HIDAPI_BASE_DIR ${CMAKE_BINARY_DIR}/hidapi/src/hidapi/)
    set(HIDAPI_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/hidapi/src/hidapi/hidapi PARENT_SCOPE)
    set(HIDAPI_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/hidapi/src/hidapi/hidapi)

    message(STATUS "HIDAPI path: ${HIDAPI_BASE_DIR}")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      set(
        lib_hidapi_LIBRARY
          ${HIDAPI_BASE_DIR}/hidapi/hidapi.h
          ${HIDAPI_BASE_DIR}/windows/hid.c
      )
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(
        lib_hidapi_LIBRARY
        ${HIDAPI_BASE_DIR}/hidapi/hidapi.h
        ${HIDAPI_BASE_DIR}/libusb/hid.c
      )
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(
        lib_hidapi_LIBRARY
        ${HIDAPI_BASE_DIR}/hidapi/hidapi.h
        ${HIDAPI_BASE_DIR}/mac/hid.c
      )
    endif()

    source_group("src" FILES  ${lib_hidapi_LIBRARY})
    add_library(hidapi STATIC ${lib_hidapi_LIBRARY})

    set_target_properties(
      hidapi
      PROPERTIES
        OUTPUT_NAME         "hidapi"
        OUTPUT_NAME_DEBUG   "hidapi${DEBUG_SUFFIX}"
    )
    target_include_directories(hidapi PUBLIC ${HIDAPI_INCLUDE_DIRS})

  endif()
endfunction()


# ------------------------------------------------------------------------------------------------ #
#  lodepng                                                                                         #
# ------------------------------------------------------------------------------------------------ #
function (addlodepng)
  if(DEFINED LODEPNG_INCLUDE_DIRS)
    message(STATUS "lodepng is already available")
  else()
    checkout_external_project(lodepng https://github.com/lvandeve/lodepng.git master)
    set(LODEPNG_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/lodepng/src/lodepng PARENT_SCOPE)
    set(LODEPNG_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/lodepng/src/lodepng)
    message(STATUS "lodepng path: ${LODEPNG_INCLUDE_DIRS}")

    set(
      lib_lodepng_LIBRARY
        ${LODEPNG_INCLUDE_DIRS}/lodepng.h
        ${LODEPNG_INCLUDE_DIRS}/lodepng.cpp
    )
    source_group("src"  FILES  ${lib_lodepng_LIBRARY})
    add_library( lodepng STATIC ${lib_lodepng_LIBRARY})

    set_target_properties(
      lodepng
      PROPERTIES
        OUTPUT_NAME         "lodepng"
        OUTPUT_NAME_DEBUG   "lodepng${DEBUG_SUFFIX}"
    )

    target_include_directories(lodepng PUBLIC ${LODEPNG_INCLUDE_DIRS})

  endif()
endfunction()

# ------------------------------------------------------------------------------------------------ #
#  nanomsgxx                                                                                         #
# ------------------------------------------------------------------------------------------------ #
function (addnanomsgxx)
  if(DEFINED NANOMSGXX_INCLUDE_DIRS)
    message(STATUS "nanomsgxx is already available")
  else()
    checkout_external_project(nanomsgxx https://github.com/achille-roussel/nanomsgxx.git master)
    set(NANOMSGXX_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/nanomsgxx/src/nanomsgxx/src PARENT_SCOPE)
    set(NANOMSGXX_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/nanomsgxx/src/nanomsgxx/src)
    message(STATUS "nanomsgxx path: ${NANOMSGXX_INCLUDE_DIRS}")

    set(
      lib_nanomsgxx_ext_LIBRARY
        ${NANOMSGXX_INCLUDE_DIRS}/nanomsg/ext/nnxx_ext.h
        ${NANOMSGXX_INCLUDE_DIRS}/nanomsg/ext/nnxx_ext.c
    )

    set(
      lib_nanomsgxx_nnxx_LIBRARY
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/error.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message_control.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message_istream.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message_iterator.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message_ostream.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message_streambuf.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/message.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/nn.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/poll.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/pubsub.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/reqrep.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/socket.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/survey.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/tcp.cpp
        ${NANOMSGXX_INCLUDE_DIRS}/nnxx/timeout.cpp
    )

    source_group("ext"  FILES  ${lib_nanomsgxx_ext_LIBRARY})
    source_group("nnxx"  FILES  ${lib_nanomsgxx_nnxx_LIBRARY})
    add_library( nanomsgxx STATIC ${lib_nanomsgxx_ext_LIBRARY} ${lib_nanomsgxx_nnxx_LIBRARY})

    set_target_properties(
      nanomsgxx
      PROPERTIES
        OUTPUT_NAME         "nanomsgxx"
        OUTPUT_NAME_DEBUG   "nanomsgxx${DEBUG_SUFFIX}"
    )

    target_include_directories(nanomsgxx PUBLIC ${NANOMSGXX_INCLUDE_DIRS})

  endif()
endfunction()
