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
)

set_target_properties(
  ${PROJECT_NAME} 
  PROPERTIES 
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} k-IOStatic)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} k-IO)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
