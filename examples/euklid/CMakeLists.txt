# -------------------------------------------------------------------------------------------------------------------- #
#  Euklid - Euclidean step sequencer                                                                                   #
# -------------------------------------------------------------------------------------------------------------------- #

project(euklid)

SET( 
  Examples_euklid_FILES      
    main.cpp
    bjorklund.h
    Sequence.h
    Euklid.h
    Euklid.cpp
)

add_executable(
  ${PROJECT_NAME} 
  ${Examples_euklid_FILES} 
  ${Libraries_RtMidi_FILES}
)

set_target_properties(
  ${PROJECT_NAME} 
  PROPERTIES 
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} kompletIOStatic)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} kompletIO)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# OS X specific ------------------------------------------------------------------------------------------------------ #

IF (APPLE)
  
  target_link_libraries(
    ${PROJECT_NAME} 
      "-framework CoreFoundation" 
      "-framework CoreAudio" 
      "-framework CoreMidi"
  )

  set_target_properties( 
    ${PROJECT_NAME} 
    PROPERTIES 
      COMPILE_DEFINITIONS __MACOSX_CORE__
  )

ENDIF (APPLE)
