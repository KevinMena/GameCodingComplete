#pragma once
#ifndef LOCALE_HANDLING_HPP
#define LOCALE_HANDLING_HPP 1

#include "os_detection/os_detection.hpp"

#include "rapidjson/encodings.h"
#include "rapidjson/stringbuffer.h"

#if IS_WIN
#include "os_detection/windows.hpp"
#include <fcntl.h>
#include <io.h>

#endif

#include <clocale>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

/* Locale Handling Namespace for constants and free functions */
namespace LocaleHandling {

/* Standard Locale Name to use */
constexpr char kStartupLocaleName[] = "en_US.UTF-8";

/* Standard Locale to use */
extern const std::locale kStartupLocale;

/* Setup application locale, must be run before any IO operation */
inline void SetStartupLocale() {
  std::setlocale(LC_ALL, kStartupLocaleName);
  std::locale::global(kStartupLocale);
  std::cin.imbue(std::locale());
  std::cout.imbue(std::locale());
  std::wcerr.imbue(std::locale());
#if IS_WIN
  // From https://stackoverflow.com/a/45622802
  // Set console code page to UTF-8 so console known how to interpret string
  // data
  // Sometimes works
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
  // This will force everything to handle manual buffering
  // setvbuf(stdout, nullptr, _IOFBF, 1000);

#endif // IS_WIN
}

#if IS_WIN

/* Container for UFT16 Strings */
using UTF16_Container = typename rapidjson::GenericStringBuffer<rapidjson::UTF16<>>;

/* Container for UFT8 Strings */
using UTF8_Container = typename rapidjson::GenericStringBuffer<rapidjson::UTF8<>>;

/* Convert a utf8 char array (not owning) to utf16. Warning: this allocates
 * memory */
UTF16_Container TranscodeToUTF16(const char *str);

/* Convert a utf8 string to utf16. Warning: this allocates memory */
inline UTF16_Container
TranscodeToUTF16(const std::string &str) {
  return TranscodeToUTF16(str.c_str());
}

/* Convert a utf16 char array (not owning) to utf8. Warning: this allocates
 * memory */
UTF8_Container TranscodeToUTF8(const wchar_t *str);

/* Convert a utf16 string to utf8. Warning: this allocates memory */
inline UTF8_Container
TranscodeToUTF8(const std::wstring &str) {
  return TranscodeToUTF8(str.c_str());
}

#else // !IS_WIN

/*
  DO NOT USE UTF16 OUTSIDE OF WINDOWS. We use UTF8 everywhere.
  Convert a utf8 char array (not owning) to utf16. Warning: this allocates
  memory
*/
rapidjson::GenericStringBuffer<rapidjson::UTF16<>>
Transcode(const char *str) = delete;

/*
  DO NOT USE UTF16 OUTSIDE OF WINDOWS. We use UTF8 everywhere.
  Convert a utf8 string to utf16. Warning: this allocates memory
*/
rapidjson::GenericStringBuffer<rapidjson::UTF16<>>
Transcode(const std::string &str) = delete;

#endif // IS_WIN

} // namespace LocaleHandling

#endif // !LOCALE_HANDLING_HPP