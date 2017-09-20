
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

# ------------------------------------------------------------------------------------------------ #
#  Device Tester                                                                                   #
# ------------------------------------------------------------------------------------------------ #

project(device-test)

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
  Examples_DeviceTest_FILES
    main.cpp
    DeviceTest.h
    DeviceTest.cpp
    ${VERSION_RESOURCES}
)

add_executable(
  ${PROJECT_NAME}
  ${Examples_DeviceTest_FILES}
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

target_link_libraries(${PROJECT_NAME} PRIVATE cabl-static)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreFoundation" "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreAudio" "-framework CoreMIDI" objc)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
