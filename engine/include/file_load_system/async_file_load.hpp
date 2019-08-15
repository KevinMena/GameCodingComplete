#pragma once
#ifndef ASYNC_FILE_LOAD_HPP
#define ASYNC_FILE_LOAD_HPP 1

#include <future>
#include <utility>

#include "file_load_system/file_load_system.hpp"

/* File Load and Writing System */
namespace FileLoadSystem {

/* Return a File, the Data requested and if there was an error */
using FileAndData = struct {
  FileLoadSystem::SmartReadFile m_file;
  std::unique_ptr<char[]> m_data;
  bool m_error = false;

  /* If the operation succeded */
  explicit operator bool() const { return !m_error; }
};

/*
  Reads size bytes of a File using a thread and saves the result to the passed
  pointer
*/
std::future<FileAndData> ReadFile(const FileLoadSystem::path &p,
                                  FileLoadSystem::file_size_t size,
                                  std::unique_ptr<char[]> ptr);

/*
  Reads a File Completely using a thread. Allocates memory to load it
*/
std::future<FileAndData> ReadFileWithAlloc(const FileLoadSystem::path &p);

} // namespace FileLoadSystem

#endif // !ASYNC_FILE_LOAD_HPP