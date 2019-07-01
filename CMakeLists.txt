cmake_minimum_required(VERSION 3.14)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

set(ENGINE_SOURCE_FOLDER engine)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(INSTALL_DIRECTORY ${CMAKE_BINARY_DIR}/${ENGINE_SOURCE_FOLDER})

project(kch_engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE KCH_ENGINE_SRC
    "${ENGINE_SOURCE_FOLDER}/src/*.c"
    "${ENGINE_SOURCE_FOLDER}/src/*.cpp"
    "${ENGINE_SOURCE_FOLDER}/src/*.cxx"
)

add_executable(kch_engine ${KCH_ENGINE_SRC})
include_directories(${ENGINE_SOURCE_FOLDER}/include)

install(TARGETS kch_engine
            RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
            LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
            ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib/static)