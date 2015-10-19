message (STATUS "Checking for HIDAPI...")
find_path(
  HIDAPI_INCLUDE_DIRS hidapi.h
  PATH_SUFFIXES hidapi
)

if(NOT WIN32)

  find_library(
    HIDAPI_LIBRARIES
    NAMES
      hidapi hidapi-libusb hidapi-hidraw
    PATHS
      /usr/lib
      /usr/local/lib
    )

endif()

if(HIDAPI_LIBRARIES AND HIDAPI_INCLUDE_DIRS)
  mark_as_advanced(HIDAPI_LIBRARIES HIDAPI_INCLUDE_DIRS)
  set(HIDAPI_FOUND true)
  message (STATUS "Found: HIDAPI")
  message(STATUS " - Includes: ${HIDAPI_INCLUDE_DIR}")
  message(STATUS " - Libraries: ${HIDAPI_LIBRARIES}")
else()
  set(HIDAPI_FOUND false)
  message (FATAL "NOT Found: HIDAPI")
endif()
