cmake_minimum_required(VERSION 3.14)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()


project(kch_engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE KCH_ENGINE_SRC
    "src/*.c"
    "src/*.cpp"
    "src/*.cxx"
)

add_executable(kch_engine ${KCH_ENGINE_SRC})
include_directories(include)