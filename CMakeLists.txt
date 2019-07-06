cmake_minimum_required(VERSION 3.14)
include(ExternalProject)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# Work Offline
option(BUILD_OFFLINE "Build offline" OFF)

if (${BUILD_OFFLINE})
    set_property(DIRECTORY ${CMAKE_BINARY_DIR}
                 PROPERTY EP_UPDATE_DISCONNECTED 1)
    message("Working offline, not pulling external projects")
endif()

# Detect Multi Config Generator 
# See: https://gitlab.kitware.com/cmake/cmake/issues/17645
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (isMultiConfig)
    set(propagate_build_type --config $<CONFIG> -DCMAKE_BUILD_TYPE=$<CONFIG>)
else()
    set(propagate_build_type -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
endif()

set(ENGINE_SOURCE_FOLDER engine)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(INSTALL_DIRECTORY ${CMAKE_BINARY_DIR}/${ENGINE_SOURCE_FOLDER})

# External Libraries
set(EXTERNAL_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}/external)

project(kch_engine)

# C++ Configuration
set(CMAKE_CXX_STANDARD 17)

# Engine Source Files
file(GLOB_RECURSE KCH_ENGINE_SRC
    "${ENGINE_SOURCE_FOLDER}/src/*.c"
    "${ENGINE_SOURCE_FOLDER}/src/*.cpp"
    "${ENGINE_SOURCE_FOLDER}/src/*.cxx"
)

# RapidJSON Config
set(RAPIDJSON ${EXTERNAL_DOWNLOAD_LOCATION}/RapidJSON)

set(RAPIDJSON_INCLUDE_PRE_LOCATION ${RAPIDJSON}/src/rapidjson-lib/include)
set(RAPIDJSON_INCLUDE_LOCATION ${RAPIDJSON}/include)

# See> https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
# See: https://stackoverflow.com/questions/38006584/how-to-clone-and-integrate-external-from-git-cmake-project-into-local-one
# See: https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
# See: https://stackoverflow.com/questions/16842218/how-to-use-cmake-externalproject-add-or-alternatives-in-a-cross-platform-way
# See: https://stackoverflow.com/questions/25520117/cmake-install-project-doesnt-copy-my-executable-to-the-folder-i-specify
ExternalProject_Add (
	  rapidjson-lib
	  PREFIX ${RAPIDJSON}
      GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
      GIT_TAG "master"
      CMAKE_GENERATOR ${CMAKE_GENERATOR}
      CMAKE_GENERATOR_TOOLSET  ${CMAKE_GENERATOR_TOOLSET}
      #UPDATE_COMMAND "" # Uncomment to stop git pull
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      )

# See: https://stackoverflow.com/questions/41384447/clean-copied-files-from-external-project
# See: https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
ExternalProject_Add_Step(
  rapidjson-lib rapidjson-copy2include
    COMMENT "Copying RapidJSON to include folder"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RAPIDJSON_INCLUDE_PRE_LOCATION} ${RAPIDJSON_INCLUDE_LOCATION}
    # See: https://stackoverflow.com/questions/41089289/externalproject-add-step-error-add-custom-command-given-append-option-with-outpu
    DEPENDEES download
)

# Targets
add_executable(kch_engine ${KCH_ENGINE_SRC})

# Set Warnings
# See: https://foonathan.net/blog/2018/10/17/cmake-warnings.html
# In case a library starts failing due to warnings, this links contains a fix
target_compile_options(kch_engine PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -Werror>
                           $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)

include_directories(${ENGINE_SOURCE_FOLDER}/include)

add_dependencies(kch_engine rapidjson-lib)
include_directories(${RAPIDJSON_INCLUDE_LOCATION})


install(TARGETS kch_engine
            RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
            LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
            ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib/static)