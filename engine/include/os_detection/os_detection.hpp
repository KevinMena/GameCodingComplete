#pragma once
#ifndef OS_DETECTION_HPP
#define OS_DETECTION_HPP 1

/* Os Detection Constants */
namespace OsDetection
{

/*
    Based on http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
*/

/* If we are compiling for Windows */
#define IS_WIN defined(_WIN32)

/* If we are compiling for Windows */
#ifdef IS_WIN
constexpr bool kIsWin = true;
#else // !IS_WIN
constexpr bool kIsWin = false;
#endif // IS_WIN



/* If we are compiling for Apple */
#define IS_APPLE (defined(__APPLE__) && defined(__MACH__))

/* If we are compiling for Apple */
#ifdef IS_APPLE
constexpr bool kIsApple = true;
#else // !IS_APPLE
constexpr bool kIsApple = false;
#endif // IS_APPLE

/* If we are compiling for Linux */
#define IS_LINUX defined(__linux__)

/* If we are compiling for Linux */
#ifdef IS_LINUX
constexpr bool kIsLinux = true;
#else // !IS_LINUX
constexpr bool kIsLinux = false;
#endif // IS_LINUX

} // namespace OsDetection
#endif // !OS_DETECTION_HPP