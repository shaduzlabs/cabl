
message (STATUS "Checking for Cairo...")
find_path(CAIRO_INCLUDE_DIRS cairo.h
  /usr/local/include/cairo
  /usr/local/include
  /usr/include/cairo
  /usr/include
)

if(NOT WIN32)

  find_library(CAIRO_LIBRARIES
    NAMES
      cairo
    PATHS
      /usr/lib
      /usr/local/lib
    )

endif()

if(CAIRO_LIBRARIES)
  mark_as_advanced(CAIRO_LIBRARIES CAIRO_INCLUDE_DIRS)
  set(CAIRO_FOUND true)
  message (STATUS "Found: Cairo")
  message(STATUS " - Includes: ${CAIRO_INCLUDE_DIRS}")
  message(STATUS " - Libraries: ${CAIRO_LIBRARIES}")
else()
  set(CAIRO_FOUND false)
  message (FATAL "NOT Found: Cairo")
endif()
