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
  /* Get Size in bytes of the compiled internals */
  virtual bool GetSize(s_size *size) = 0;
  /* Get Length (amount of chars used) of the compiled internals */
  virtual bool GetLength(s_size *length) = 0;
  /* Put the compiled internals in a char array*/
  virtual bool GetText(char *text) = 0;

  /*
  Open an space for a new entry with name and version.
  If it is at the same level as an opened array, the name is not used and the
  entry is appended to the array
  */
  virtual bool SenEntry(const char *name, s_size version) = 0;
  /*
  Gets in the space of an entry and return its version
  If it is at the same level as an opened array, the name is not used and the
  array entry is opened
  */
  virtual bool OpenEntry(const char *name, s_size *version) const = 0;
  /* Close an entry */
  virtual bool Close() const = 0;

  /*
  Sets a new bool
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetBool(const char *name, bool value) = 0;
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
  Sets a new unsigned entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetUint(const char *name, unsigned value) = 0;
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
  Sets a new int entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetInt(const char *name, int value) = 0;
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
  Sets a new uint64_t entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetUint64(const char *name, uint64_t value) = 0;
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
  Sets a new int64_t entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetInt64(const char *name, int64_t value) = 0;
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
  Sets a new double entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetDouble(const char *name, double value) = 0;
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
  Sets a new string entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetString(const char *name, const char *value, s_size size) = 0;
  /*
  Gets if the current/named entry is a string
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsString(const char *name) const = 0;
  /*
  Gets the named or current string entry length
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetStringLength(const char *name, s_size *size) const = 0;
  /*
  Gets the named or current string entry
  If it is at the same level as an opened array, the name is not used and it
  gets the current opened entry is
  */
  virtual bool GetString(const char *name, const char **value);

  /*
  Sets a new null entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetNull(const char *name) = 0;
  /*
  Gets if the current/named entry is null
  If it is at the same level as an opened array, the name is not used and it
  checks if the current opened entry is
  */
  virtual bool IsNull(const char *name) const = 0;

  /*
  Sets a new array entry
  If it is at the same level as an opened array, the name is not used and it
  is appended to the array
  */
  virtual bool SetArray(const char *name) = 0;
  /*
  Reserve Array memory, might be hard to predict due to recursions
  */
  virtual bool ReserveArray(s_size size) = 0;
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
  /* Get Current Array Capacity */
  virtual bool GetArrayCapacity(s_size *size) const = 0;
  /* Moves the Current Array Iterator. It starts at begin(). This function will
   * return false when it hits end() */
  virtual bool MoveArray() const = 0;
  /* Close an array entry */
  virtual bool CloseArray() const = 0;
};

} // namespace Serializer

#endif // !SERIALIZER_HPP