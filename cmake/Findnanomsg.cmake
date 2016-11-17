message (STATUS "Checking for nanomsg...")

if(NOT DEFINED NANOMSG_ROOT)
	set(NANOMSG_ROOT /usr /usr/local $ENV{NANOMSG_ROOT})
endif(NOT DEFINED NANOMSG_ROOT)

find_path(NANOMSG_INCLUDE_DIRS
  nanomsg/nn.h
  PATHS ${NANOMSG_ROOT}
  PATH_SUFFIXES nanomsg/include include
)

if(WIN32)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a .dll ${CMAKE_FIND_LIBRARY_SUFFIXES})
else()
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .dylib .so)
endif()

find_library(NANOMSG_LIBRARIES
	NAMES nanomsg
	PATHS ${NANOMSG_ROOT}
	PATH_SUFFIXES lib bin
)

if(NANOMSG_LIBRARIES AND NANOMSG_INCLUDE_DIRS)
  mark_as_advanced(NANOMSG_LIBRARIES NANOMSG_INCLUDE_DIRS)
  set(NANOMSG_FOUND true)
  message (STATUS "Found: nanomsg")
  message(STATUS " - Includes: ${NANOMSG_INCLUDE_DIRS}")
  message(STATUS " - Libraries: ${NANOMSG_LIBRARIES}")
else()
  set(NANOMSG_FOUND false)
  message (WARNING "NOT Found: nanomsg")
endif()
