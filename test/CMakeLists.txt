include_directories( ${PROJECT_SOURCE_DIR}/submodules/catch/include )

project(unit-tests)
enable_testing(true)

SET( 
  test_SRCS
    main.cpp
)

SET( 
  test_util_SRCS
    util/Version.cpp
    util/MidiMessage.cpp
)

source_group(""                  FILES ${test_SRCS})
source_group("util"              FILES ${test_util_SRCS})

SET( 
  Test_FILES      
    ${test_SRCS}
    ${test_util_SRCS}
)

add_executable(
  ${PROJECT_NAME} 
  ${Test_FILES} 
)

set_target_properties(
  ${PROJECT_NAME} 
  PROPERTIES 
    OUTPUT_NAME           ${PROJECT_NAME}
    OUTPUT_NAME_DEBUG     ${PROJECT_NAME}${DEBUG_SUFFIX}
)

target_link_libraries(${PROJECT_NAME} k-IOStatic)
