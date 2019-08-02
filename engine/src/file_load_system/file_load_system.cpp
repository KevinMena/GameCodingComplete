#include "file_load_system/file_load_system.hpp"

#include "file_load_system/smart_file.hpp"
#include "locale_handling/locale_handling.hpp"
#include "os_detection/os_detection.hpp"

#if IS_LINUX
#include <limits.h>
#include <unistd.h>
#endif // IS_LINUX

#if IS_WIN
#include "os_detection/windows.hpp"
#endif // IS_WIN

#include <filesystem>
#include <system_error>

#if IS_WIN
// Windows.h bleeds macros all over the place from filesystem
#undef GetCurrentDirectory
#endif // IS_WIN

namespace FileLoadSystem {

constexpr uint8_t utf8BOM[] = {0xEF, 0xBB, 0xBF};

/* Process UTF8 BOM from read file */
bool ProcessReadUTF8BOM(std::FILE *f) {
  if (std::fgetc(f) != utf8BOM[0]) {
    std::rewind(f);
    return true;
  }

  if (std::fgetc(f) != utf8BOM[1]) {
    std::rewind(f);
    return true;
  }

  if (std::fgetc(f) != utf8BOM[2]) {
    std::rewind(f);
    return true;
  }

  return true;
}

/* Add UTF8 BOM to write File */
bool ProcessWriteUTF8BOM(std::FILE *f) {
  if (std::fputc(utf8BOM[0], f) == EOF) {
    return false;
  }

  if (std::fputc(utf8BOM[1], f) == EOF) {
    return false;
  }

  if (std::fputc(utf8BOM[2], f) == EOF) {
    return false;
  }
  return true;
}

/* Add UTF8 BOM to Append File */
bool ProcessAppendUTF8BOM(std::FILE *f) {
  FSeek(f, 0, SEEK_END);
  if (FTell(f) == 0) {
    return ProcessWriteUTF8BOM(f);
  }

  std::rewind(f);
  return true;
}

SmartReadFile OpenReadBinary(const path &p) {

#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"rb");

  if (res != 0) {
    return {};
  }

  SmartReadFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartReadFile f(std::fopen(p.c_str(), "rb"));
#endif // !IS_WIN

  return f;
}

SmartReadFile OpenReadText(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"rt");

  if (res != 0) {
    return {};
  }

  SmartReadFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartReadFile f(std::fopen(p.c_str(), "rt"));
#endif // !IS_WIN

  // Check if Valid for UTF8 BOM Processing
  if (f.IsValid()) {
    // Check for UTF8 BOM
    if (!ProcessReadUTF8BOM(f.Get())) {
      return {};
    }
  }

  return f;
}

SmartWriteFile OpenWriteBinary(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"wb");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "wb"));
#endif // !IS_WIN
  return f;
}

SmartWriteFile OpenWriteText(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"wt");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "wt"));
#endif // !IS_WIN

  // Check if the file opened
  if (f.IsValid()) {
    // Add the UTF8 BOM
    if (!ProcessWriteUTF8BOM(f.Get())) {
      return {};
    }
  }

  return f;
}

SmartWriteFile OpenAppendBinary(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"ab");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "ab"));
#endif // !IS_WIN
  return f;
}

SmartWriteFile OpenAppendText(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"at");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "at"));
#endif // !IS_WIN

  // Check if the file opened
  if (f.IsValid()) {
    // Add the UTF8 BOM
    if (!ProcessAppendUTF8BOM(f.Get())) {
      return {};
    }
  }

  return f;
}

/* Open a Binary File for Append */
SmartWriteFile OpenReadAppendBinary(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"r+b");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "r+b"));
#endif // !IS_WIN
  return f;
}

/* Open a Binary File for Append */
SmartWriteFile OpenReadAppendText(const path &p) {
#if IS_WIN
  std::FILE *file = nullptr;
  errno_t res = _wfopen_s(&file, p.c_str(), L"r+t");

  if (res != 0) {
    return {};
  }

  SmartWriteFile f(file);
// IS_WIN
#else  // !IS_WIN
  SmartWriteFile f(std::fopen(p.c_str(), "r+t"));
#endif // !IS_WIN

  // Check if the file opened
  if (f.IsValid()) {
    // Add the UTF8 BOM
    if (!ProcessAppendUTF8BOM(f.Get())) {
      return {};
    }
  }
  return f;
}

/* Temporary Path */
path kTempPath;

const path &GetTempDirectory() { return kTempPath; }

/* Current Path */
path kCurrentPath;

const path &GetCurrentDirectory() { return kCurrentPath; }

path kExecutableName;

const path &GetExecutableName() { return kExecutableName; }

/* Executable Path */
path kExecutablePath;

const path &GetExecutableDirectory() { return kExecutablePath; }

error_status SetupFileLoadSystem(const char *studio_name,
                                 const char *game_name) {
  error_status error;
  kCurrentPath = std::filesystem::current_path(error);

  if (error) {
    return error;
  }

  kTempPath = std::filesystem::temp_directory_path(error);

  if (error) {
    return error;
  }

  kTempPath /= FileLoadSystem::CreatePath(studio_name) /
               FileLoadSystem::CreatePath(game_name);

#if IS_WIN

  {
    // See:
    // https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
    wchar_t buffer[MAX_PATH + 1];
    DWORD size = GetModuleFileNameW(NULL, buffer, MAX_PATH);

    if (size == 0) {
      return std::make_error_code(std::errc::state_not_recoverable);
    }

    // This wont work on WinXP
    // See:
    // https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      return std::make_error_code(std::errc::not_enough_memory);
    }

    LocaleHandling::UTF8_Container fixedBuffer =
        LocaleHandling::TranscodeToUTF8(buffer);

    kExecutableName = CreatePath(fixedBuffer.GetString());
    kExecutablePath = kExecutableName.parent_path();
  }

// IS_WIN
#elif IS_LINUX

  {
    // https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
    // PATH_MAX might be dangerous. See https://stackoverflow.com/a/38353197
    char buff[PATH_MAX + 1];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);

    if (len == -1) {
      return std::make_error_code(std::errc::state_not_recoverable);
    }

    buff[len] = '\0';

    kExecutableName = CreatePath(buff);
    kExecutablePath = kExecutableName.parent_path();
  }

#else // !IS_WIN && !IS_LINUX
#error This Platform is not yet supported to get paths
#endif

  return error;
}

} // namespace FileLoadSystem