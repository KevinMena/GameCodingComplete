#include "file_load_system/file_load_system.hpp"

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

error_status SetupFileLoadSystem(const char* studio_name, const char* game_name) {
  error_status error;
  kCurrentPath = std::filesystem::current_path(error);

  if (error) {
    return error;
  }

  kTempPath = std::filesystem::temp_directory_path(error);

  if (error) {
    return error;
  }

  kTempPath /= FileLoadSystem::CreatePath(studio_name) / FileLoadSystem::CreatePath(game_name);

#if IS_WIN

  {
    // See: https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
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