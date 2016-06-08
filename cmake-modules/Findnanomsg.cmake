message (STATUS "Checking for nanomsg...")
find_path(NANOMSG_INCLUDE_DIRS nanomsg/nn.h
  /usr/local/include
  /usr/include
)

if(NOT WIN32)

  find_library(NANOMSG_LIB_SHARED
    NAMES
      libnanomsg.dylib
      libnanomsg.so
    PATHS
      /usr/local/lib
      /usr/lib
    NO_DEFAULT_PATH
  )

  find_library(NANOMSG_LIB_STATIC
    NAMES
      libnanomsg.a
    PATHS
      /usr/local/lib
      /usr/lib
    NO_DEFAULT_PATH
  )

endif()

if(NANOMSG_INCLUDE_DIRS AND NANOMSG_LIB_SHARED AND NANOMSG_LIB_STATIC)
  set(NANOMSG_FOUND true)

  mark_as_advanced(NANOMSG_INCLUDE_DIRS NANOMSG_LIB_SHARED NANOMSG_LIB_STATIC)
  add_definitions(-DNANOMSG)
  set(NANOMSG_LIBRARIES "${NANOMSG_LIB_SHARED};${NANOMSG_LIB_STATIC}")

  message(STATUS "Found: nanomsg")
  message(STATUS " - Includes: ${NANOMSG_INCLUDE_DIRS}")
  message(STATUS " - Libraries: ${NANOMSG_LIBRARIES}")
else()
  set(NANOMSG_FOUND false)
message (FATAL "NOT Found: nanomsg")
endif()
