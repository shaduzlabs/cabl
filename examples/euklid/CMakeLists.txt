project(euklid)

SET( 
     Examples_euklid_FILES      main.cpp
                                bjorklund.h
                                Sequence.h
                                Euklid.h
                                Euklid.cpp
                                lib/RtMidi.h
                                lib/RtMidi.cpp
)

add_executable(${PROJECT_NAME} ${Examples_euklid_FILES} )
set_target_properties(kompletIO PROPERTIES 
                                OUTPUT_NAME ${PROJECT_NAME}
                                OUTPUT_NAME_DEBUG ${PROJECT_NAME}-d)

target_link_libraries(${PROJECT_NAME} kompletIO)

IF (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation" "-framework CoreAudio" "-framework CoreMidi")
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS __MACOSX_CORE__)
ENDIF (APPLE)
