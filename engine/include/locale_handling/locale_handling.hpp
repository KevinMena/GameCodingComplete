#pragma once
#ifndef LOCALE_HANDLING_HPP
#define LOCALE_HANDLING_HPP 1

#include "os_detection/os_detection.hpp"

#if IS_WIN
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

#endif

#include <clocale>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <locale>


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

  // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
  // This will force everything to handle manual buffering
  // setvbuf(stdout, nullptr, _IOFBF, 1000);

#endif // IS_WIN
}

} // namespace LocaleHandling

#endif // !LOCALE_HANDLING_HPP