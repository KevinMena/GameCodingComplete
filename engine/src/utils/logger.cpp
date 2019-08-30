#include "utils/logger.hpp"

#include <cstdarg>
#include <ctime>

#include "file_load_system/file_load_system.hpp"

namespace Logging {

/* Inspired by https://github.com/rxi/log.c/blob/master/src/log.c */
void Logger::Log(const char *originFile, int line, int level, const char *fmt,
                 ...) const {
  if (m_quiet || (m_level != -1 && m_level < level)) {
    return;
  }

  std::lock_guard<std::mutex> guard(m_mutex);

  /* Get current time */
  time_t t = time(nullptr);
  struct tm lt;
  if (localtime_s(&lt, &t) != 0) {
    std::fprintf(m_file, "Failed_Time %s:%d (%d): ", originFile, line, level);
  } else {
    char buf[32];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", &lt)] = '\0';
    std::fprintf(m_file, "%s %s:%d (%d): ", buf, originFile, line, level);
  }

  va_list args;
  va_start(args, fmt);
  vfprintf(m_file, fmt, args);
  va_end(args);

  fprintf(m_file, "\n");
  fflush(m_file);
}

/* Logging System */

const std::string Logger::s_kDefaultLoggerName = "default";
const std::string Logger::s_kDefaultErrorLoggerName = "error";
bool Logger::s_systemActive = false;
std::mutex Logger::s_mutex;
std::unordered_map<std::string, std::shared_ptr<Logger>> Logger::s_loggerDB;
FileLoadSystem::path Logger::s_commonPath;
std::shared_ptr<Logger> Logger::s_defaultLogger;
std::shared_ptr<Logger> Logger::s_defaultErrorLogger;

std::shared_ptr<Logger> Logger::GetLogger(const std::string &name) {
  std::lock_guard<std::mutex> guard(s_mutex);
  auto iter = s_loggerDB.find(name);
  if (iter != s_loggerDB.end()) {
    return iter->second;
  }
  return {};
}

std::shared_ptr<Logger> Logger::CreateLogger(std::string name, int level) {
  std::lock_guard<std::mutex> guard(s_mutex);
  auto iter = s_loggerDB.find(name);
  if (iter != s_loggerDB.end()) {
    return iter->second;
  }

  FileLoadSystem::SmartWriteFile file =
      FileLoadSystem::OpenAppendText(s_commonPath / (name + ".txt"));

  if (!file.IsValid()) {
    return {};
  }

  std::FILE *realFile = file.Get();
  file.Reset();

  std::shared_ptr<Logger> logger = std::make_shared<Logger>();

  logger->Init(std::make_shared<FileLoadSystem::SmartWriteFile>(realFile),
               level);

  s_loggerDB.insert(std::make_pair(std::move(name), logger));
  return logger;
}

std::shared_ptr<Logger> Logger::CreateLogger(std::string name, std::FILE *file,
                                             int level) {
  std::lock_guard<std::mutex> guard(s_mutex);
  auto iter = s_loggerDB.find(name);
  if (iter != s_loggerDB.end()) {
    return iter->second;
  }

  if (file == nullptr) {
    return {};
  }

  std::shared_ptr<Logger> logger = std::make_shared<Logger>();

  logger->Init(file, level);

  s_loggerDB.insert(std::make_pair(std::move(name), logger));
  return logger;
}

std::shared_ptr<Logger>
Logger::CreateLogger(std::string name,
                     std::shared_ptr<FileLoadSystem::SmartWriteFile> file,
                     int level) {
  std::lock_guard<std::mutex> guard(s_mutex);
  auto iter = s_loggerDB.find(name);
  if (iter != s_loggerDB.end()) {
    return iter->second;
  }

  if (file.get() == nullptr) {
    return {};
  }

  std::shared_ptr<Logger> logger = std::make_shared<Logger>();

  logger->Init(std::move(file), level);

  s_loggerDB.insert(std::make_pair(std::move(name), logger));
  return logger;
}

std::shared_ptr<Logger> Logger::ConnectLogger(std::string name,
                                              std::shared_ptr<Logger> logger) {
  s_loggerDB.insert_or_assign(std::move(name), logger);
  return logger;
}

bool Logger::SetupLogging(const FileLoadSystem::path &commomPath) {
  std::lock_guard<std::mutex> guard(s_mutex);
  CommonSetupLogging(commomPath);

  s_defaultLogger = CreateLogger(s_kDefaultLoggerName, stdout);
  s_defaultErrorLogger = CreateLogger(s_kDefaultErrorLoggerName, stderr);

  if (s_defaultLogger.get() == nullptr ||
      s_defaultErrorLogger.get() == nullptr) {
    return false;
  }

  s_systemActive = true;
  return true;
}

bool Logger::SetupLogging(const FileLoadSystem::path &commomPath,
                          std::FILE *default, std::FILE *defaultError) {
  std::lock_guard<std::mutex> guard(s_mutex);
  CommonSetupLogging(commomPath);

  s_defaultLogger = CreateLogger(s_kDefaultLoggerName, default);
  s_defaultErrorLogger = CreateLogger(s_kDefaultErrorLoggerName, defaultError);

  if (s_defaultLogger.get() == nullptr ||
      s_defaultErrorLogger.get() == nullptr) {
    return false;
  }

  s_systemActive = true;
  return true;
}

bool Logger::SetupLogging(
    const FileLoadSystem::path &commomPath,
    std::shared_ptr<FileLoadSystem::SmartWriteFile> default,
    std::FILE *defaultError) {
  std::lock_guard<std::mutex> guard(s_mutex);
  CommonSetupLogging(commomPath);

  s_defaultLogger = CreateLogger(s_kDefaultLoggerName, default);
  s_defaultErrorLogger = CreateLogger(s_kDefaultErrorLoggerName, defaultError);

  if (s_defaultLogger.get() == nullptr ||
      s_defaultErrorLogger.get() == nullptr) {
    return false;
  }

  s_systemActive = true;
  return true;
}

bool Logger::SetupLogging(
    const FileLoadSystem::path &commomPath, std::FILE *default,
    std::shared_ptr<FileLoadSystem::SmartWriteFile> defaultError) {
  std::lock_guard<std::mutex> guard(s_mutex);
  CommonSetupLogging(commomPath);

  s_defaultLogger = CreateLogger(s_kDefaultLoggerName, default);
  s_defaultErrorLogger = CreateLogger(s_kDefaultErrorLoggerName, defaultError);

  if (s_defaultLogger.get() == nullptr ||
      s_defaultErrorLogger.get() == nullptr) {
    return false;
  }

  s_systemActive = true;
  return true;
}

bool Logger::SetupLogging(
    const FileLoadSystem::path &commomPath,
    std::shared_ptr<FileLoadSystem::SmartWriteFile> default,
    std::shared_ptr<FileLoadSystem::SmartWriteFile> defaultError) {
  std::lock_guard<std::mutex> guard(s_mutex);
  CommonSetupLogging(commomPath);

  s_defaultLogger = CreateLogger(s_kDefaultLoggerName, default);
  s_defaultErrorLogger = CreateLogger(s_kDefaultErrorLoggerName, defaultError);

  if (s_defaultLogger.get() == nullptr ||
      s_defaultErrorLogger.get() == nullptr) {
    return false;
  }

  s_systemActive = true;
  return true;
}

} // namespace Logging
