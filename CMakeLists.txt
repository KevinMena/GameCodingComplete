cmake_minimum_required(VERSION 3.14)
include(ExternalProject)

# https://stackoverflow.com/questions/36122358/visual-studio-code-c-include-path
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

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

# Detect 64bit Build
# See: https://stackoverflow.com/questions/39258250/how-to-detect-if-64-bit-msvc-with-cmake
set(USE_64 ON)
if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" 
	AND "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4" 
	AND NOT CMAKE_CL_64)
    set(USE_64 OFF)
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

project(kch_engine)
include(CTest)

# External Libraries
set(EXTERNAL_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}/external)
set(EXTERNAL_PRE_DOWNLOAD_LOCATION ${PROJECT_SOURCE_DIR}/external)

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

# Debug Assert Config
set(DEBUG_ASSERT ${EXTERNAL_DOWNLOAD_LOCATION}/DEBUG_ASSERT)
set(DEBUG_ASSERT_DOWNLOAD ${DEBUG_ASSERT}/debug-assert-download)
set(DEBUG_ASSERT_SRC ${DEBUG_ASSERT}/debug-assert-src)
set(DEBUG_ASSERT_BUILD ${DEBUG_ASSERT}/debug-assert-build)
set(DEBUG_ASSERT_INCLUDE_LOCATION ${DEBUG_ASSERT}/include)
ExternalProject_Add(debug_assert-lib
  GIT_REPOSITORY    https://github.com/foonathan/debug_assert.git
  GIT_TAG           master
  SOURCE_DIR        "${DEBUG_ASSERT_SRC}"
  BINARY_DIR        "${DEBUG_ASSERT_BUILD}"
  INSTALL_COMMAND   ""
)

# See: https://stackoverflow.com/questions/41384447/clean-copied-files-from-external-project
# See: https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
ExternalProject_Add_Step(
  debug_assert-lib debug_assert-copy2include
    COMMENT "Copying debug_assert to include folder"
    COMMAND "${CMAKE_COMMAND}" -E copy "${DEBUG_ASSERT_SRC}/debug_assert.hpp" "${DEBUG_ASSERT_INCLUDE_LOCATION}/debug_assert.hpp"
    # See: https://stackoverflow.com/questions/41089289/externalproject-add-step-error-add-custom-command-given-append-option-with-outpu
    DEPENDEES download
)

# GLFW
set(GLFW_LIB_NAME lib64)
set(GLFW_LIB_SUFFIX 64)
if(NOT USE_64)
   set(GLFW_LIB_NAME lib)
   set(GLFW_LIB_SUFFIX "")
endif()

set(GLFW_LOCATION ${EXTERNAL_DOWNLOAD_LOCATION}/GLFW)

set(GLFW_INCLUDE_LOCATION ${GLFW_LOCATION}/include)
set(GLFW_LIB_LOCATION ${GLFW_LOCATION}/${GLFW_LIB_NAME})
set(GLFW_BIN_LOCATION ${GLFW_LOCATION}/bin)


# See> https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
# See: https://stackoverflow.com/questions/38006584/how-to-clone-and-integrate-external-from-git-cmake-project-into-local-one
# See: https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
# See: https://stackoverflow.com/questions/16842218/how-to-use-cmake-externalproject-add-or-alternatives-in-a-cross-platform-way
# See: https://stackoverflow.com/questions/25520117/cmake-install-project-doesnt-copy-my-executable-to-the-folder-i-specify
ExternalProject_Add (
	  glfw-lib
	  PREFIX ${GLFW_LOCATION}
      GIT_REPOSITORY "https://github.com/glfw/glfw.git"
      GIT_TAG "master"
      CMAKE_GENERATOR ${CMAKE_GENERATOR}
      CMAKE_GENERATOR_TOOLSET  ${CMAKE_GENERATOR_TOOLSET}
      #UPDATE_COMMAND "" # Uncomment to stop git pull
      INSTALL_COMMAND # See https://stackoverflow.com/questions/16842218/how-to-use-cmake-externalproject-add-or-alternatives-in-a-cross-platform-way
        ${CMAKE_COMMAND}
        --build .
        --target install
      CMAKE_ARGS 
    	  -DCMAKE_INSTALL_PREFIX=${GLFW_LOCATION}
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      	#-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        ${propagate_build_type}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
      	-DLIB_SUFFIX=${GLFW_LIB_SUFFIX} 
      	-DBUILD_SHARED_LIBS=ON 
      	-DGLFW_BUILD_TESTS=OFF 
      	-DGLFW_BUILD_EXAMPLES=OFF 
      	-DGLFW_BUILD_DOCS=ON
      )

# GLM 
set(GLM_LOCATION ${EXTERNAL_DOWNLOAD_LOCATION}/GLM)
set(GLM_INCLUDE_PRE_LOCATION ${GLM_LOCATION}/src/glm-lib/glm)
set(GLM_INCLUDE_LOCATION ${GLM_LOCATION}/include)
ExternalProject_Add (
	  glm-lib
	  PREFIX ${GLM_LOCATION}
      GIT_REPOSITORY "https://github.com/g-truc/glm.git"
      GIT_TAG "master"
      #UPDATE_COMMAND "" # Uncomment to stop git pull
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      CMAKE_GENERATOR ${CMAKE_GENERATOR}
      CMAKE_GENERATOR_TOOLSET  ${CMAKE_GENERATOR_TOOLSET}
      CMAKE_ARGS 
    	-DCMAKE_INSTALL_PREFIX=${GLM_LOCATION}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      )

# See: https://stackoverflow.com/questions/41384447/clean-copied-files-from-external-project
# See: https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
ExternalProject_Add_Step(
    glm-lib glm-copy2include
    COMMENT "Copying GLM to include folder"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLM_INCLUDE_PRE_LOCATION} ${GLM_INCLUDE_LOCATION}/glm
    # See: https://stackoverflow.com/questions/41089289/externalproject-add-step-error-add-custom-command-given-append-option-with-outpu
    DEPENDEES download
)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${GLM_INCLUDE_LOCATION}"
)

# Testing
if(${BUILD_TESTING})
    set(KCH_ENGINE_FOR_TEST_SRC  ${KCH_ENGINE_SRC})
    # https://stackoverflow.com/questions/15550777/how-do-i-exclude-a-single-file-from-a-cmake-fileglob-pattern
    # DO NOT CHANGE NAME OF MAIN
    list(FILTER KCH_ENGINE_FOR_TEST_SRC EXCLUDE REGEX ".*/main.cpp$")

    # Engine Test Source Files
    file(GLOB_RECURSE KCH_ENGINE_TEST_SRC
        "${ENGINE_SOURCE_FOLDER}/test_src/*.c"
        "${ENGINE_SOURCE_FOLDER}/test_src/*.cpp"
        "${ENGINE_SOURCE_FOLDER}/test_src/*.cxx"
    )

    set(EXTERNAL_TEST_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}/test_external)
    set(TESTS_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
    set(TESTS_RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIRECTORY}/bin)
    set(TESTS_ARCHIVE_OUTPUT_DIRECTORY ${TESTS_DIRECTORY}/lib/static)
    set(TESTS_LIBRARY_OUTPUT_DIRECTORY ${TESTS_DIRECTORY}/lib)

    # Download and unpack googletest at configure time
    # https://github.com/google/googletest/blob/master/googletest/README.md#using-cmake
    set(GTEST_DOWNLOAD_FOLDER ${EXTERNAL_TEST_DOWNLOAD_LOCATION}/googletest-download)

    configure_file(gtest_config/CMakeLists.txt.in ${GTEST_DOWNLOAD_FOLDER}/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${GTEST_DOWNLOAD_FOLDER} )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${GTEST_DOWNLOAD_FOLDER})
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${EXTERNAL_TEST_DOWNLOAD_LOCATION}/googletest-src
                    ${EXTERNAL_TEST_DOWNLOAD_LOCATION}/googletest-build
                    EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    set_target_properties(gmock_main PROPERTIES PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock_main PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock_main PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${TESTS_ARCHIVE_OUTPUT_DIRECTORY})
    set_target_properties(gmock_main PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TESTS_LIBRARY_OUTPUT_DIRECTORY})

    set_target_properties(gmock PROPERTIES PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gmock PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${TESTS_ARCHIVE_OUTPUT_DIRECTORY})
    set_target_properties(gmock PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TESTS_LIBRARY_OUTPUT_DIRECTORY})

    set_target_properties(gtest_main PROPERTIES PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest_main PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest_main PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${TESTS_ARCHIVE_OUTPUT_DIRECTORY})
    set_target_properties(gtest_main PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TESTS_LIBRARY_OUTPUT_DIRECTORY})

    set_target_properties(gtest PROPERTIES PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})
    set_target_properties(gtest PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${TESTS_ARCHIVE_OUTPUT_DIRECTORY})
    set_target_properties(gtest PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TESTS_LIBRARY_OUTPUT_DIRECTORY})
    
endif()

# Targets
# https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html
add_executable(kch_engine ${KCH_ENGINE_SRC})

# Unicode is UTF8. Accept no substitutes
add_compile_options($<$<C_COMPILER_ID:MSVC>:/utf-8>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# Set Warnings
# See: https://foonathan.net/blog/2018/10/17/cmake-warnings.html
# In case a library starts failing due to warnings, this links contains a fix
target_compile_options(kch_engine PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -Werror>
                           $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /DUNICODE=1 /D_UNICODE=1>)


if(${BUILD_TESTING})
    add_executable(kch_engine_test ${KCH_ENGINE_TEST_SRC} ${KCH_ENGINE_FOR_TEST_SRC})

    set_target_properties(kch_engine_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_RUNTIME_OUTPUT_DIRECTORY})    

    target_compile_options(kch_engine_test PRIVATE
                           $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -Werror>
                           $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)

    add_dependencies(kch_engine_test kch_engine)
    target_include_directories(kch_engine_test PUBLIC ${ENGINE_SOURCE_FOLDER}/test_include)

    # https://github.com/google/googletest/blob/master/googlemock/README.md
    target_link_libraries(kch_engine_test gmock_main)
    add_test(kch_engine_test COMMAND kch_engine_test)
endif()

include_directories(${ENGINE_SOURCE_FOLDER}/include)

add_dependencies(kch_engine rapidjson-lib)
include_directories(${RAPIDJSON_INCLUDE_LOCATION})

add_dependencies(kch_engine debug_assert-lib)
include_directories(${DEBUG_ASSERT_INCLUDE_LOCATION})

add_dependencies(kch_engine glfw-lib)
include_directories(${GLFW_INCLUDE_LOCATION})

add_dependencies(kch_engine glm-lib)
include_directories(${GLM_INCLUDE_LOCATION})

# GLEW (Only for windows for now)
if(WIN32)
    set(GLEW_FOLDER ${EXTERNAL_PRE_DOWNLOAD_LOCATION}/glew)
    set(GLEW_ARCH x64)

    # check if we are building for win32
    if(USE_64)
        set(GLEW_ARCH Win32)
    endif()

    set(GLEW_INCLUDE ${GLEW_FOLDER}/include)
    set(GLEW_LIB ${GLEW_FOLDER}/lib/Release/${GLEW_ARCH})
    set(GLEW_BIN ${GLEW_FOLDER}/bin/Release/${GLEW_ARCH})

    include_directories(${GLEW_INCLUDE})
    link_directories(${GLEW_LIB})

    add_custom_command(TARGET kch_engine POST_BUILD        # Adds a post-build event to kch_engine
    COMMENT "Copying glew dll to include folder"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${GLEW_BIN}/glew32.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:kch_engine>/glew32.dll)                 # <--this is out-file path


else()
    message( FATAL_ERROR "GLEW only supported for windows for now")
endif()


install(TARGETS kch_engine
            RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
            LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
            ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib/static)

install(FILES 
    $<TARGET_FILE_DIR:kch_engine>/${CMAKE_SHARED_MODULE_PREFIX}glfw3${CMAKE_SHARED_LIBRARY_SUFFIX}
    DESTINATION ${INSTALL_DIRECTORY}/bin)