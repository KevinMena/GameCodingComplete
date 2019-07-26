#include "file_load_system/file_load_system.hpp"

#include "os_detection/os_detection.hpp"
#include "locale_handling/locale_handling.hpp"

#ifdef IS_WIN
#include "os_detection/windows.hpp"
#endif // IS_WIN

#include <filesystem>
#include <system_error>

#if IS_WIN
// Windows.h bleeds macros all over the place from filesystem
#undef GetCurrentDirectory
#endif // IS_WIN

namespace FileLoadSystem {

/* Current Path */
path kCurrentPath;

const path &GetCurrentDirectory() {
  return kCurrentPath;
}

path kExecutableName;

const path &GetExecutableName() {
  return kExecutableName;
}

/* Executable Path */
path kExecutablePath;

const path &GetExecutableDirectory() {
  return kExecutablePath;
}


error_status SetupFileLoadSystem() {
    error_status error;
    kCurrentPath = std::filesystem::current_path(error);

    if( error ) {
      return error;
    }

#if IS_WIN

  {
    wchar_t buffer[MAX_PATH+1]; 
    DWORD size = GetModuleFileNameW(NULL, buffer, MAX_PATH) ;

    if(size == 0) {
      return std::make_error_code(std::errc::state_not_recoverable);
    }

    // This wont work on WinXP
    // See: https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      return std::make_error_code(std::errc::not_enough_memory);
    }

    LocaleHandling::UTF8_Container fixedBuffer = LocaleHandling::TranscodeToUTF8(buffer);

    kExecutableName = CreatePath(fixedBuffer.GetString());
    kExecutablePath = kExecutableName.parent_path();
    
  }

#endif // IS_WIN

    return error;
}

} // namespace FileLoadSystem