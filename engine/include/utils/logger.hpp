#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP 1

#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include "file_load_system/file_load_system.hpp"
#include "file_load_system/smart_file.hpp"

/* Logging related things */
namespace Logging {

/* Logger Hanlder */
class Logger {
public:
  Logger() = default;
  Logger(Logger &&) = default;
  Logger(const Logger &) = delete;
  Logger &operator=(Logger &&) = default;
  Logger &operator=(const Logger &) = delete;
  ~Logger() { Log(__FILE__, __LINE__, 0, "Closed Log"); }

  /* Get Logger Level */
  inline int GetLevel() const {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_level;
  }

  /* Set Logger Level. A level of -1 means everything goes */
  inline void SetLevel(int level) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_level = level;
  }

  /* Get Quiet Mode */
  inline bool GetQuiet() const {
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_quiet;
  }

  /* Set Quiet Mode */
  inline void SetQuiet(bool active) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_quiet = active;
  }

  // TODO: add a way to retrieve and change underlying file

  /* Log information, including Origin File, line log level*/
  void Log(const char *originFile, int line, int level, const char *fmt,
           ...) const;

  /* Change Logger associated File */
  inline void ChangeFile(std::shared_ptr<FileLoadSystem::SmartWriteFile> file) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_protectedFile = std::move(file);
    m_file = m_protectedFile.get()->Get();
  }

  /* Change Logger associated File */
  inline void ChangeFile(std::FILE *file) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_protectedFile.reset();
    m_file = file;
  }

  /* Logging System */

  /* Get a Logger with a name */
  static std::shared_ptr<Logger> GetLogger(const std::string &name);

  /* Create a Logger with a name (Logging System handles file creation) and
   * level. If it already exists return the existing */
  static std::shared_ptr<Logger> CreateLogger(std::string name, int level = -1);

  /* Create a Logger with a name with a non owning file (Logging System is not
   * responsible) with a level. If it already exists return the existing */
  static std::shared_ptr<Logger> CreateLogger(std::string name, std::FILE *file,
                                              int level = -1);

  /* Create a Logger with a name with a shared file with a level. If it already
   * exists return the existing */
  static std::shared_ptr<Logger>
  CreateLogger(std::string name,
               std::shared_ptr<FileLoadSystem::SmartWriteFile> file,
               int level = -1);

  /* Connect two names to the same logger a new logger. It doesn't update
   * previous references */
  static std::shared_ptr<Logger> ConnectLogger(std::string name,
                                               std::shared_ptr<Logger> logger);

  /* Setup Logging system using commonPath as the folder to save logs. Default
   * logger goes to stdout and Default Error Logger goes to stderr. MUST BE
   * CALLED BEFORE ANY OTHER CALL TO THE SYSTEM. Returns true if successful */
  static bool SetupLogging(const FileLoadSystem::path &commomPath);

  /* Setup Logging system using commonPath as the folder to save logs. Setup the
   * specific file for default logger and error logger. MUST BE CALLED BEFORE
   * ANY OTHER CALL TO THE SYSTEM. Returns true if successful */
  static bool SetupLogging(const FileLoadSystem::path &commomPath,
                           std::FILE *default, std::FILE *defaultError);

  /* Setup Logging system using commonPath as the folder to save logs. Setup the
   * specific file for default logger and error logger. MUST BE CALLED BEFORE
   * ANY OTHER CALL TO THE SYSTEM. Returns true if successful */
  static bool
  SetupLogging(const FileLoadSystem::path &commomPath,
               std::shared_ptr<FileLoadSystem::SmartWriteFile> default,
               std::FILE *defaultError);

  /* Setup Logging system using commonPath as the folder to save logs. Setup the
   * specific file for default logger and error logger. MUST BE CALLED BEFORE
   * ANY OTHER CALL TO THE SYSTEM. Returns true if successful */
  static bool
  SetupLogging(const FileLoadSystem::path &commomPath, std::FILE *default,
               std::shared_ptr<FileLoadSystem::SmartWriteFile> defaultError);

  /* Setup Logging system using commonPath as the folder to save logs. Setup the
   * specific file for default logger and error logger. MUST BE CALLED BEFORE
   * ANY OTHER CALL TO THE SYSTEM. Returns true if successful */
  static bool
  SetupLogging(const FileLoadSystem::path &commomPath,
               std::shared_ptr<FileLoadSystem::SmartWriteFile> default,
               std::shared_ptr<FileLoadSystem::SmartWriteFile> defaultError);

  /* End the Logging System. All the loggers are still valid but its UB to
   * register or ask for another */
  inline static void FinishLogging() {
    std::lock_guard<std::mutex> guard(s_mutex);
    s_loggerDB.clear();
    s_systemActive = false;
  }

  /* Get Default Logger */
  inline static std::shared_ptr<Logger> GetDefaultLogger() {
    return s_defaultLogger;
  }

  /* Get Default Error Logger */
  inline static std::shared_ptr<Logger> GetDefaultErrorLogger() {
    return s_defaultErrorLogger;
  }

  /* Get Default Logger Name*/
  inline static const std::string &GetDefaultLoggerName() {
    return s_kDefaultLoggerName;
  }

  /* Get Default Error Logger Name*/
  inline static const std::string &GetDefaultErrorLoggerName() {
    return s_kDefaultErrorLoggerName;
  }

private:
  /* Init Logger with associated File and Level. Must be done before any call to
   * the logger */
  inline void Init(std::shared_ptr<FileLoadSystem::SmartWriteFile> file,
                   int level = -1) {
    m_protectedFile = std::move(file);
    m_file = m_protectedFile.get()->Get();
    m_level = level;
    Log(__FILE__, __LINE__, 0, "Logger Started");
  }

  /* Init Logger with associated File (Non protected) and Level. Must be done
   * before any call to the logger */
  inline void Init(std::FILE *file, int level = -1) {
    m_file = file;
    m_level = level;
    Log(__FILE__, __LINE__, 0, "Logger Started");
  }

  /* Logging level */
  int m_level = -1;
  /* Set the logger to quiet mode */
  bool m_quiet = false;
  /* Keep File alive for the duration of the logger */
  std::shared_ptr<FileLoadSystem::SmartWriteFile> m_protectedFile;
  /* Logger output file */
  std::FILE *m_file;
  /* Log sync */
  mutable std::mutex m_mutex;

  /* Static System */

  /* Common Setup for Loggin System */
  inline static void
  CommonSetupLogging(const FileLoadSystem::path &commomPath) {
    s_commonPath = commomPath;
    s_loggerDB.reserve(s_kStartReserve);
  }

  /* Initial reserve to hold loggers */
  constexpr static size_t s_kStartReserve = 10;

  /* Default Logger Name */
  const static std::string s_kDefaultLoggerName;

  /* Default Error Logger Name */
  const static std::string s_kDefaultErrorLoggerName;

  /* If the logger system is active */
  static bool s_systemActive;

  /* Entry point mutex */
  static std::mutex s_mutex;

  /* DB of all existing loggers */
  static std::unordered_map<std::string, std::shared_ptr<Logger>> s_loggerDB;

  /* Common Path for all the loggers files handled by the system */
  static FileLoadSystem::path s_commonPath;

  /* Default Logger */
  static std::shared_ptr<Logger> s_defaultLogger;

  /* Default Error Logger */
  static std::shared_ptr<Logger> s_defaultErrorLogger;
};

/* Log something */
#define LOG(logger, level, msg, ...)                                           \
  logger->Log(__FILE__, __LINE__, level, msg, __VA_ARGS__)

} // namespace Logging

#endif // !LOGGER_HPP