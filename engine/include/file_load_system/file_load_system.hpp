#pragma once
#ifndef FILE_LOAD_SYSTEM_HPP
#define FILE_LOAD_SYSTEM_HPP 1

#include "file_load_system/smart_file.hpp"
#include "os_detection/os_detection.hpp"

#if IS_WIN
#include "os_detection/windows.hpp"
#endif // IS_WIN

#if IS_LINUX
#include <sys/types.h>
#endif // IS_LINUX

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

/* Create Directories recursively for every element of path p that needs it  */
inline bool CreateDirectories(const path &p, error_status &error) {
  return std::filesystem::create_directories(p, error);
}

/* Remove a File or an Empty Directory */
inline bool Remove(const path &p, error_status &error) {
  return std::filesystem::remove(p, error);
}

/*
  Remove a Directory with its content Recursively.
  Returns the number of files and directories that were deleted (which may be
  zero if p did not exist to begin with). The overload that takes error_code&
  argument returns static_cast<std::uintmax_t>(-1) on error
*/
inline std::uintmax_t RemoveDirectory(const path &p, error_status &error) {
  return std::filesystem::remove_all(p, error);
}

/* Copies a File or a Directory (Non Recursively) */
inline void Copy(const path &from, const path &to, error_status &error) {
  return std::filesystem::copy(from, to, error);
}

/* Copies a Directory Recursively */
inline void CopyRecursive(const path &from, const path &to,
                          error_status &error) {
  return std::filesystem::copy(from, to,
                               std::filesystem::copy_options::recursive, error);
}

/* Moves or renames a File or a Directory */
inline void Rename(const path &from, const path &to, error_status &error) {
  return std::filesystem::rename(from, to, error);
}

/* ftell for Large Files */
inline std::uintmax_t FTell(std::FILE *f) {

#if IS_WIN
  return static_cast<std::uintmax_t>(_ftelli64(f));
// IS_WIN
#elif IS_LINUX
  return static_cast<std::uintmax_t>(ftello(f));
// IS_LINUX
#else
#error Platform Not Supported Yet
#endif // else
}

/* Seek Set as a constant */
constexpr int kSeekSet = SEEK_SET;

/* Seek Cur as a constant */
constexpr int kSeekCur = SEEK_CUR;

/* Seek Cur as a constant */
constexpr int kSeekEnd = SEEK_END;

/* fseek for Large Files */
inline int FSeek(std::FILE *f, std::uintmax_t offset, int origin) {

#if IS_WIN
  return _fseeki64(f, static_cast<__int64>(offset), origin);
// IS_WIN
#elif IS_LINUX
  return fseeko(f, static_cast<off_t>(offset), origin);
// IS_LINUX
#else
#error Platform Not Supported Yet
#endif // else
}

/* Open a Binary File for Read */
SmartReadFile OpenReadBinary(const path &p);

/* Open a Text File for Read */
SmartReadFile OpenReadText(const path &p);

/* Open a Binary File for Write */
SmartWriteFile OpenWriteBinary(const path &p);

/* Open a Text File for Write */
SmartWriteFile OpenWriteText(const path &p);

/* Open a Binary File for Append */
SmartWriteFile OpenAppendBinary(const path &p);

/* Open a Text File for Append */
SmartWriteFile OpenAppendText(const path &p);

/* Open a Binary File for Append */
SmartWriteFile OpenReadAppendBinary(const path &p);

/* Open a Text File for Append */
SmartWriteFile OpenReadAppendText(const path &p);

/* If the file reached EOF */
inline bool Feof(std::FILE *f) { return std::feof(f) != 0; }

/* IF there is an error on the File processing */
inline bool Ferror(std::FILE *f) { return std::ferror(f) != 0; }

/*
  Reads an array of count elements, each one with a size of size bytes, from the
  stream and stores them in the block of memory specified by ptr.

  The position indicator of the stream is advanced by the total amount of bytes
  read.

  The total amount of bytes read if successful is (size*count).
 */
inline size_t Fread(void *ptr, size_t size, size_t count, FILE *stream) {
  return std::fread(ptr, size, count, stream);
}

/*
  Writes an array of count elements, each one with a size of size bytes, from
  the block of memory pointed by ptr to the current position in the stream.

  The position indicator of the stream is advanced by the total number of bytes
  written.

  Internally, the function interprets the block pointed by ptr as if it was an
  array of (size*count) elements of type unsigned char, and writes them
  sequentially to stream as if fputc was called for each byte.
*/
inline size_t Fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
  return std::fwrite(ptr, size, count, stream);
}

/* Get Directory for Temp Files. It should not fail on common filesystems */
const path &GetTempDirectory();

/* Get Current Directory Path. Warning: This is NOT where the executable is */
const path &GetCurrentDirectory();

/* Get Executable Directory Path. */
const path &GetExecutableDirectory();

/* Get Executable Name Path. */
const path &GetExecutableName();

/*
  Sets up the File Load System. Must be called before any File Operation
  It needs Studio Name and Game Name for Temporary Folders Names
*/
error_status SetupFileLoadSystem(const char *studio_name,
                                 const char *game_name);

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

  /* Remove a File or an Empty Directory */
  inline bool Remove(const path &p) {
    return FileLoadSystem::Remove(p, m_error);
  }

  /*
    Remove a Directory Recursively.
    Returns the number of files and directories that were deleted (which may be
    zero if p did not exist to begin with). The overload that takes error_code&
    argument returns static_cast<std::uintmax_t>(-1) on error
  */
  inline std::uintmax_t RemoveDirectory(const path &p) {
    return FileLoadSystem::RemoveDirectory(p, m_error);
  }

  /* Copies a File or a Directory (Non Recursively) */
  inline void Copy(const path &from, const path &to) {
    return FileLoadSystem::Copy(from, to, m_error);
  }

  /* Copies a Directory Recursively */
  inline void CopyRecursive(const path &from, const path &to) {
    return FileLoadSystem::CopyRecursive(from, to, m_error);
  }

  /* Moves or renames a File or a Directory */
  inline void Rename(const path &from, const path &to) {
    return FileLoadSystem::Rename(from, to, m_error);
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