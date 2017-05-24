
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

include(ExternalProject)

# This function is used to force a build on a dependant project at cmake configuration phase.
# see:
# http://stackoverflow.com/questions/17446981/cmake-externalproject-add-and-findpackage/23570741
function (checkout_external_project target repository tag)
    message(STATUS "Checking out external repository: ${target}")
    set(trigger_build_dir ${PROJECT_BINARY_DIR}/${target})

    #mktemp dir in build tree
    file(MAKE_DIRECTORY ${trigger_build_dir} ${trigger_build_dir}/build)

    #generate false dependency project
    set(CMAKE_LIST_CONTENT "
      cmake_minimum_required(VERSION 2.8)

      include(ExternalProject)
      ExternalProject_add(
        ${target}
        PREFIX ${PROJECT_BINARY_DIR}/${target}
        GIT_REPOSITORY ${repository}
        GIT_TAG ${tag}
        CONFIGURE_COMMAND echo \"\"
        BUILD_COMMAND echo \"\"
          LOG_DOWNLOAD ON
        LOG_INSTALL ON
        CMAKE_ARGS ${ARGN}
        INSTALL_COMMAND \"\"
      )

      add_custom_target(trigger_${target})
      add_dependencies(trigger_${target} ${target})"
    )

    file(WRITE ${trigger_build_dir}/CMakeLists.txt "${CMAKE_LIST_CONTENT}")

    execute_process(COMMAND ${CMAKE_COMMAND} ..
        WORKING_DIRECTORY ${trigger_build_dir}/build
        )
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${trigger_build_dir}/build
        )

endfunction()
