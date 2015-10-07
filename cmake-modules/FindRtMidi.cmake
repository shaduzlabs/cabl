
message (STATUS "Checking for RtMidi...")
find_path(RTMIDI_INCLUDE_DIR RtMidi.h
  /usr/local/include
  /usr/include
)

if(NOT WIN32)

  find_library(RTMIDI_LIBRARY
    NAMES
      rtmidi
    PATHS
      /usr/lib
      /usr/local/lib
    )

endif()

if(RTMIDI_LIBRARY)
  mark_as_advanced(RTMIDI_LIBRARY RTMIDI_INCLUDE_DIR)

if(APPLE)
  set(RTMIDI_LINKER_FLAGS "-framework CoreFoundation" "-framework IOKit" "-framework CoreAudio")
  set(RTMIDI_LINKER_FLAGS ${RTMIDI_LINKER_FLAGS} "-framework CoreMidi" objc ${RTMIDI_LIBRARY})
else()
  set(RTMIDI_LINKER_FLAGS ${RTMIDI_LINKER_FLAGS} "-framework CoreMidi" objc ${RTMIDI_LIBRARY})
endif()

  set(RTMIDI_FOUND true)
  message (STATUS "Found: RtMidi")
  message(STATUS " - Includes: ${RTMIDI_INCLUDE_DIR}")
  message(STATUS " - Libraries: ${RTMIDI_LIBRARY}")
else()
  set(RTMIDI_FOUND false)
  message (FATAL "NOT Found: RtMidi")
endif()
