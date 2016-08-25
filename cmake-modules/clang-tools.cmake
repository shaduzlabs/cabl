# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
     *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
     )

find_program(CLANG_FORMAT "clang-format")
find_program(CLANG_TIDY "clang-tidy")

# Adding clang-format target if executable is found
if(CLANG_FORMAT)
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT}
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
    )
  message(STATUS "Clang Format found, target will be generated.")
  set_target_properties(clang-format PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# Adding clang-tidy target if executable is found
function(clangTidyTarget target_)
  if(CLANG_TIDY AND TARGET ${target_})
    get_property(TARGET_SOURCE_FILES TARGET ${target_} PROPERTY SOURCES)
    set(TARGET_SOURCES "")
    foreach(source_file ${TARGET_SOURCE_FILES})
      set(TARGET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${source_file} ${TARGET_SOURCES}")
    endforeach()

    get_property(TARGET_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    set(TARGET_INCLUDES "")
    foreach(dir ${TARGET_INCLUDE_DIRECTORIES})
      set(TARGET_INCLUDES "-I${dir} ${TARGET_INCLUDES}")
    endforeach()

    get_property(TARGET_SOURCE_FILES TARGET ${target_} PROPERTY SOURCES)
    get_property(TARGET_INCLUDE_DIRECTORIES TARGET ${target_} PROPERTY INCLUDE_DIRECTORIES)
    add_custom_target(
      clang-tidy
      COMMAND ${CLANG_TIDY}
      -config=''
      ${TARGET_SOURCE_FILES}
      --
      ${TARGET_INCLUDES}
      -std=c++11
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )
    message(STATUS "Clang Tidy found, target will be generated.")
    set_target_properties(clang-tidy PROPERTIES EXCLUDE_FROM_ALL TRUE)
  endif()
endfunction()
