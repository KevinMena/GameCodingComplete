
#include "file_load_system/async_file_load.hpp"
#include "file_load_system/file_load_system.hpp"
#include "file_load_system/smart_file.hpp"


namespace FileLoadSystem {

/*
  Reads a File Completely and saves the result to the passed pointer
*/
std::unique_ptr<char[]> InternalReadFile(const FileLoadSystem::path &p,
                                         file_size_t size,
                                         std::unique_ptr<char[]> ptr) {
  FileLoadSystem::SmartReadFile file = FileLoadSystem::OpenReadText(p);

  if (!file.IsValid()) {
    return std::unique_ptr<char[]>();
  }

  FileLoadSystem::Fread(ptr.get(), sizeof(char), size, file.Get());

  return ptr;
}

/*
  Reads a File Completely and allocates memory to load it
*/
std::unique_ptr<char[]> InternalReadFileAlloc(const FileLoadSystem::path &p) {
  FileLoadSystem::error_status err;
  file_size_t size = FileLoadSystem::FileSize(p, err);

  if (err) {
    return std::unique_ptr<char[]>();
  }

  std::unique_ptr<char[]> res(
      static_cast<char *>(std::malloc(size * sizeof(char) + 1)));

  return InternalReadFile(p, size, std::move(res));
}

std::future<std::unique_ptr<char[]>> ReadFile(const FileLoadSystem::path &p,
                                              file_size_t size,
                                              std::unique_ptr<char[]> ptr) {
  return std::async(std::launch::async, InternalReadFile, p, size,
                    std::move(ptr));
}

std::future<std::unique_ptr<char[]>>
ReadFileWithAlloc(const FileLoadSystem::path &p) {
  return std::async(std::launch::async, InternalReadFileAlloc, p);
}

} // namespace FileLoadSystem