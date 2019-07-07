#pragma once
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP 1

#include <cstdint>
#include <string>

namespace Serializer {
/* Type for sizes (arrays, strings, etc...) */
using s_size = typename std::string::size_type;

/* Handle Serialization of Things */
class ISerializer {
public:
  /* Parse from Text */
  virtual bool ParseText(const char *text) = 0;
  /* Clear internals */
  virtual bool Clear() = 0;
  /* Compile internals */
  virtual bool Compile() = 0;
  /* Compile internals in a Pretty Format*/
  virtual bool CompilePretty() = 0;
  /* Get Size in bytes of the compiled internals */
  virtual bool GetSize(s_size *size) = 0;
  /* Get Length (amount of chars used) of the compiled internals */
  virtual bool GetLength(s_size *length) = 0;
  /* Put the compiled internals in a char array. It should have at least enough space */
  virtual bool GetText(char *text) = 0;

  /*
  Open an space for a new entry with name and version. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and the
  entry is appended to the array
  */
  virtual bool SetEntry(const char *name, s_size name_length, s_size version) = 0;
  /*
  Gets in the space of an entry and return its version
  If it is at the same level as an opened array, the name is not used and the
  array entry is opened
  */
  virtual bool OpenEntry(const char *name, s_size *version) const = 0;
  /* Close an entry */
  virtual bool CloseEntry() const = 0;

  /*
  Sets a new bool. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetBool(const char *name, s_size name_length, bool value) = 0;
  /*
  Gets if the current/named entry is a bool
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsBool(const char *name) const = 0;
  /*
  Gets the named or current bool entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetBool(const char *name, bool *result) const = 0;

  /*
  Sets a new unsigned entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetUint(const char *name, s_size name_length, unsigned value) = 0;
  /*
  Gets if the current/named entry is an unsigned
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsUint(const char *name) const = 0;
  /*
  Gets the named or current unsigned entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetUint(const char *name, unsigned *result) const = 0;

  /*
  Sets a new int entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetInt(const char *name, s_size name_length, int value) = 0;
  /*
  Gets if the current/named entry is an int
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsInt(const char *name) const = 0;
  /*
  Gets the named or current int entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetInt(const char *name, int *result) const = 0;

  /*
  Sets a new uint64_t entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetUint64(const char *name, s_size name_length, uint64_t value) = 0;
  /*
  Gets if the current/named entry is an uint64_t
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsUint64(const char *name) const = 0;
  /*
  Gets the named or current uint64_t entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetUint64(const char *name, uint64_t *result) const = 0;

  /*
  Sets a new int64_t entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetInt64(const char *name, s_size name_length, int64_t value) = 0;
  /*
  Gets if the current/named entry is an int64_t
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsInt64(const char *name) const = 0;
  /*
  Gets the named or current int64_t entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetInt64(const char *name, int64_t *result) const = 0;

  /*
  Sets a new double entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetDouble(const char *name, s_size name_length, double value) = 0;
  /*
  Gets if the current/named entry is an double
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsDouble(const char *name) const = 0;
  /*
  Gets the named or current double entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetDouble(const char *name, double *result) const = 0;

  /*
  Sets a new string entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetString(const char *name, s_size name_length, const char *value, s_size length) = 0;
  /*
  Gets if the current/named entry is a string
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsString(const char *name) const = 0;
  /*
  Gets the named or current string entry length (UTF makes this tricky)
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetStringLength(const char *name, s_size *result) const = 0;
  /*
  Gets the named or current string size in bytes
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetStringSize(const char *name, s_size *result) const = 0;
  /*
  Gets the named or current string entry. The result pointer must be able to hold the string
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetString(const char *name, char *result) const = 0;

  /*
  Sets a new null entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetNull(const char *name, s_size name_length) = 0;
  /*
  Gets if the current/named entry is null
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsNull(const char *name) const = 0;

  /*
  Sets a new array entry. Length excludes null terminator.
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetArray(const char *name, s_size name_length) = 0;
  /*
  Reserve Array memory, might be hard to predict due to recursions
  */
  virtual bool ReserveArray(const char *name, s_size size) = 0;
  /*
  Gets if the current/named entry is an array
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsArray(const char *name) const = 0;
  /*
  Gets in the space of an entry
  If it is at the same level as an opened array, the name is not used and the
  array entry is opened
  */
  virtual bool OpenArray(const char *name) const = 0;
  /* Get Current/Named Array Capacity */
  virtual bool GetArrayCapacity(const char *name, s_size *size) const = 0;
  /* Moves the Current Array Iterator. It starts at begin(). This function will
   * return false when it hits end() */
  virtual bool MoveArray() const = 0;
  /* Close an array entry */
  virtual bool CloseArray() const = 0;

  /* Virtual Destructor */
  virtual ~ISerializer() = default;
};

} // namespace Serializer

#endif // !SERIALIZER_HPP