#pragma once
#ifndef ASYNC_FILE_LOAD_HPP
#define ASYNC_FILE_LOAD_HPP 1

#include <future>
#include <utility>

#include "file_load_system/file_load_system.hpp"

/* File Load and Writing System */
namespace FileLoadSystem {

/*
  Reads size bytes of a File using a thread and saves the result to the passed
  pointer
*/
std::future<std::unique_ptr<char[]>> ReadFile(const FileLoadSystem::path &p,
                                              FileLoadSystem::file_size_t size,
                                              std::unique_ptr<char[]> ptr);

/*
  Reads a File Completely using a thread and allocates memory to load it
*/
std::future<std::unique_ptr<char[]>>
ReadFileWithAlloc(const FileLoadSystem::path &p);

} // namespace FileLoadSystem

#endif // !ASYNC_FILE_LOAD_HPP