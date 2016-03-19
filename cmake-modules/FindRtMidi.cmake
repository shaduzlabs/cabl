
message (STATUS "Checking for RtMidi...")
find_path(RTMIDI_INCLUDE_DIRS RtMidi.h
  /usr/local/include
  /usr/include
)

if(NOT WIN32)

  find_library(RTMIDI_LIBRARIES
    NAMES
      rtmidi
    PATHS
      /usr/lib
      /usr/local/lib
    )

endif()

if(RTMIDI_LIBRARIES)
  mark_as_advanced(RTMIDI_LIBRARIES RTMIDI_INCLUDE_DIRS)

if(APPLE)
  set(RTMIDI_LINKER_FLAGS "-framework CoreFoundation" "-framework IOKit" "-framework CoreAudio")
  set(RTMIDI_LINKER_FLAGS ${RTMIDI_LINKER_FLAGS} "-framework CoreMidi" objc ${RTMIDI_LIBRARIES})
else()
  set(RTMIDI_LINKER_FLAGS ${RTMIDI_LINKER_FLAGS} "-framework CoreMidi" objc ${RTMIDI_LIBRARIES})
endif()

  set(RTMIDI_FOUND true)
  message (STATUS "Found: RtMidi")
  message(STATUS " - Includes: ${RTMIDI_INCLUDE_DIRS}")
  message(STATUS " - Libraries: ${RTMIDI_LIBRARIES}")
else()
  set(RTMIDI_FOUND false)
  message (FATAL "NOT Found: RtMidi")
endif()
