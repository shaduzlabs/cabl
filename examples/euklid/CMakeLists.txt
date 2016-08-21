
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Euklid - Euclidean step sequencer                                                               #
# ------------------------------------------------------------------------------------------------ #

project(euklid)

set(VERSION_RESOURCES "")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  generateProductVersion(
     VERSION_RESOURCES
     NAME           ${PROJECT_NAME}
     ICON           "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico"
     VERSION_MAJOR  ${CABL_VERSION_MAJOR}
     VERSION_MINOR  ${CABL_VERSION_MINOR}
     VERSION_PATCH  ${CABL_VERSION_MICRO}
     COMPANY_NAME   "Vincenzo Pacella"
  )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

SET(
  Examples_euklid_FILES
    main.cpp
    bjorklund.h
    Sequence.h
    Euklid.h
    Euklid.cpp
    ${VERSION_RESOURCES}
)

add_executable(
  ${PROJECT_NAME}
  ${Examples_euklid_FILES}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

if(RTMIDI_LOCAL_BUILD)
  target_link_libraries( ${PROJECT_NAME} PUBLIC rtmidi)
else()
  target_link_libraries( ${PROJECT_NAME} PUBLIC ${RTMIDI_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE cabl-static)
