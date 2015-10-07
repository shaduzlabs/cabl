message (STATUS "Checking for HIDAPI...")
find_path(
  HIDAPI_INCLUDE_DIR hidapi.h
  PATH_SUFFIXES hidapi
)

if(NOT WIN32)

  find_library(
    HIDAPI_LIBRARY
    NAMES
      hidapi hidapi-libusb hidapi-hidraw
    PATHS
      /usr/lib
      /usr/local/lib
    )

endif()

if(HIDAPI_LIBRARY AND HIDAPI_INCLUDE_DIR)
  mark_as_advanced(HIDAPI_LIBRARY HIDAPI_INCLUDE_DIR)
  set(HIDAPI_FOUND true)
  message (STATUS "Found: HIDAPI")
  message(STATUS " - Includes: ${HIDAPI_INCLUDE_DIR}")
  message(STATUS " - Libraries: ${HIDAPI_LIBRARY}")
else()
  set(HIDAPI_FOUND false)
  message (FATAL "NOT Found: HIDAPI")
endif()
