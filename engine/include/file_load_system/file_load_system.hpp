#pragma once
#ifndef FILE_LOAD_SYSTEM_HPP
#define FILE_LOAD_SYSTEM_HPP 1

#include "os_detection/os_detection.hpp"

#ifdef IS_WIN
#include "os_detection/windows.hpp"
#endif // IS_WIN

#include <cstdint>
#include <filesystem>
#include <system_error>
#include <utility>

#if IS_WIN
// Windows.h bleeds macros all over the place from filesystem
#undef GetCurrentDirectory
#endif // IS_WIN

/* File Load and Writing System */
namespace FileLoadSystem {

/* Path Representation */
using path = typename std::filesystem::path;

using error_status = std::error_code;

/* Size Error Constant. Check for this or you are going to blow up the memory */
constexpr std::uintmax_t kSizeError = static_cast<std::uintmax_t>(-1);

/*
  Create a filesystem path from an utf8 source.
  It can use char* and std::string.
  For more see std::filesystem for arguments
*/
template <typename... Args> inline path CreatePath(Args &&... args) {
  // This will change for C++20
  // TODO: Fix this for C++20
  return std::filesystem::u8path(std::forward<Args...>(args...));
}

/* If a File/Folder Exists */
inline bool Exists(const path &p, error_status &error) {
  return std::filesystem::exists(p, error);
}

/* Get File Size */
inline std::uintmax_t FileSize(const path &p, error_status &error) {
  return std::filesystem::file_size(p, error);
}

/* Get Directory for Temp Files. It should not fail on common filesystems */
const path &GetTempDirectory();

/* Get Current Directory Path. Warning: This is NOT where the executable is */
const path &GetCurrentDirectory();

/* Get Executable Directory Path. */
const path &GetExecutableDirectory();

/* Get Executable Name Path. */
const path &GetExecutableName();

/* Create Directories recursively for every element of path p that needs it  */
inline bool CreateDirectories(const path &p, error_status &error) {
  return std::filesystem::create_directories(p, error);
}

/* Sets up the File Load System. Must be called before any File Operation */
error_status SetupFileLoadSystem();

/* Handle File Querys. This class is not thread safe. It is designed for an IO
 * Thread */
class FileLoadSystemHandler {
public:
  /*
  Create a filesystem path from an utf8 source.
  It can use char* and std::string.
  For more see std::filesystem for arguments
  */
  template <typename... Args> inline path CreatePath(Args &&... args) const {
    return FileLoadSystem::CreatePath<Args...>(std::forward<Args...>(args...));
  }

  /* If a File/Folder Exists. Not thread safe */
  inline bool Exists(const path &p) const {
    return FileLoadSystem::Exists(p, m_error);
  }

  /* If a File Size. Not thread safe */
  inline std::uintmax_t FileSize(const path &p) const {
    return FileLoadSystem::FileSize(p, m_error);
  }

  /* Create Directories recursively for every element of path p that needs it */
  inline bool CreateDirectories(const path &p) const {
    return FileLoadSystem::CreateDirectories(p, m_error);
  }

  /* Default Constructor and Destructor */
  FileLoadSystemHandler() = default;
  ~FileLoadSystemHandler() = default;

  /* Move Semantics */
  FileLoadSystemHandler(FileLoadSystemHandler &&) = default;
  FileLoadSystemHandler &operator=(FileLoadSystemHandler &&) = default;

  /* Copy is not allowed because it doesn't make sense */
  FileLoadSystemHandler(const FileLoadSystemHandler &) = delete;
  FileLoadSystemHandler &operator=(const FileLoadSystemHandler &) = delete;

  /* Get Current Error Status */
  const error_status &GetErrorStatus() const { return m_error; }

private:
  /* Error code to avoid exceptions */
  mutable error_status m_error;
};

/* THINK: Maybe replace some of the code with direct filesystem calls */

} // namespace FileLoadSystem

#endif // !FILE_LOAD_SYSTEM_HPP