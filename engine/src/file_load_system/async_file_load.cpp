
#include "file_load_system/async_file_load.hpp"
#include "file_load_system/file_load_system.hpp"
#include "file_load_system/smart_file.hpp"

#include <utility>

namespace FileLoadSystem {

/*
  Reads a File and saves the result to the passed pointer
*/
FileAndData InternalReadFileNoOpen(FileLoadSystem::SmartReadFile file,
                                   file_size_t size,
                                   std::unique_ptr<char[]> ptr) {
  FileAndData res;
  res.m_file = std::move(file);
  res.m_data = std::move(ptr);

  if (!res.m_file.IsValid()) {
    res.m_error = true;
    return res;
  }

  FileLoadSystem::Fread(res.m_data.get(), sizeof(char), size, res.m_file.Get());

  return res;
}

/*
  Reads a File and saves the result to the passed pointer
*/
FileAndData InternalReadFile(const FileLoadSystem::path &p, file_size_t size,
                             std::unique_ptr<char[]> ptr) {
  FileLoadSystem::SmartReadFile file = FileLoadSystem::OpenReadText(p);

  FileAndData res;
  res.m_file = std::move(file);
  res.m_data = std::move(ptr);

  if (!res.m_file.IsValid()) {
    res.m_error = true;
    return res;
  }

  FileLoadSystem::Fread(res.m_data.get(), sizeof(char), size, res.m_file.Get());

  return res;
}

/*
  Reads a File Completely and allocates memory to load it
*/
FileAndData InternalReadFileAlloc(const FileLoadSystem::path &p) {
  FileLoadSystem::error_status err;
  file_size_t size = FileLoadSystem::FileSize(p, err);

  if (err) {
    FileAndData res;
    res.m_error = true;
    return res;
  }

  std::unique_ptr<char[]> data(
      static_cast<char *>(std::malloc(size * sizeof(char) + 1)));

  return InternalReadFile(p, size, std::move(data));
}

std::future<FileAndData> ReadFile(FileLoadSystem::SmartReadFile file,
                                  FileLoadSystem::file_size_t size,
                                  std::unique_ptr<char[]> ptr) {
  return std::async(std::launch::async, InternalReadFileNoOpen, std::move(file),
                    size, std::move(ptr));
}

std::future<FileAndData> ReadFile(const FileLoadSystem::path &p,
                                  file_size_t size,
                                  std::unique_ptr<char[]> ptr) {
  return std::async(std::launch::async, InternalReadFile, p, size,
                    std::move(ptr));
}

std::future<FileAndData> ReadFileWithAlloc(const FileLoadSystem::path &p) {
  return std::async(std::launch::async, InternalReadFileAlloc, p);
}

} // namespace FileLoadSystem