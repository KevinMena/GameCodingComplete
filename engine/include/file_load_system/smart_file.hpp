#pragma once
#ifndef SMART_FILE_HPP
#define SMART_FILE_HPP 1

#include <cstdio>

namespace FileLoadSystem {

/* RAII Class for FILEs that are written */
class SmartWriteFile {
public:
  SmartWriteFile() { m_file = nullptr; }
  /* Protect File f */
  SmartWriteFile(std::FILE *f) { m_file = f; }
  inline SmartWriteFile(SmartWriteFile &&other) noexcept {
    if (m_file != nullptr) {
      std::fflush(m_file);
      std::fclose(m_file);
      m_file = nullptr;
    }
    m_file = other.m_file;
    other.m_file = nullptr;
  }
  SmartWriteFile(const SmartWriteFile &) = delete;
  inline SmartWriteFile &operator=(SmartWriteFile &&other) noexcept {
    if (m_file != nullptr) {
      std::fflush(m_file);
      std::fclose(m_file);
      m_file = nullptr;
    }
    m_file = other.m_file;
    other.m_file = nullptr;
    return *this;
  }
  SmartWriteFile &operator=(const SmartWriteFile &) = delete;
  inline ~SmartWriteFile() {
    if (m_file != nullptr) {
      std::fflush(m_file);
      std::fclose(m_file);
      m_file = nullptr;
    }
  }

  /* Get File pointer */
  inline std::FILE *Get() { return m_file; }

  /* If the File pointer is valid */
  inline bool IsValid() const { return m_file != nullptr; }

  /* Releases Ownership of file without closing it and points to nullptr */
  inline void Reset() {
    if (m_file != nullptr) {
      std::fflush(m_file);
    }
    m_file = nullptr;
  }

private:
  std::FILE *m_file = nullptr;
};

/* RAII Class for FILEs that are read */
class SmartReadFile {
public:
  SmartReadFile() { m_file = nullptr; }
  /* Protect File f */
  inline SmartReadFile(std::FILE *f) { m_file = f; }
  inline SmartReadFile(SmartReadFile &&other) noexcept {
    if (m_file != nullptr) {
      std::fclose(m_file);
      m_file = nullptr;
    }
    m_file = other.m_file;
    other.m_file = nullptr;
  }
  SmartReadFile(const SmartReadFile &) = delete;
  inline SmartReadFile &operator=(SmartReadFile &&other) noexcept {
    if (m_file != nullptr) {
      std::fclose(m_file);
      m_file = nullptr;
    }
    m_file = other.m_file;
    other.m_file = nullptr;
    return *this;
  }
  SmartReadFile &operator=(const SmartReadFile &) = delete;
  inline ~SmartReadFile() {
    if (m_file != nullptr) {
      std::fclose(m_file);
      m_file = nullptr;
    }
  }

  /* Get File pointer */
  inline std::FILE *Get() { return m_file; }

  /* If the File pointer is valid */
  inline bool IsValid() const { return m_file != nullptr; }

  /* Releases Ownership of file without closing it and points to nullptr */
  inline void Reset() { m_file = nullptr; }

private:
  std::FILE *m_file = nullptr;
};

} // namespace FileLoadSystem

#endif // !SMART_FILE_HPP