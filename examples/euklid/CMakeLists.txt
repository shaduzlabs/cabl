SET( 
     Examples_euklid_FILES      main.cpp
                                bjorklund.h
                                Sequence.h
                                Euklid.h
                                Euklid.cpp
                                RtMidi.h
                                RtMidi.cpp
)

add_executable(euklid ${Examples_euklid_FILES} )
target_link_libraries(euklid kompletIO)

IF (APPLE)
  target_link_libraries(euklid "-framework CoreFoundation" "-framework CoreAudio" "-framework CoreMidi")
  set_target_properties(euklid PROPERTIES COMPILE_DEFINITIONS __MACOSX_CORE__)
ENDIF (APPLE)
