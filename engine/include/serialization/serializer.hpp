#pragma once
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP 1

#include <cstdint>
#include <string>

namespace Serializer
{
    
    class ISerializer
    {
    public:
        /* Open */
        virtual bool Open(const char* name, const char* version) = 0;
        virtual bool Close() = 0;

        virtual bool SetBool(const char* name, bool value) = 0;
        virtual bool IsBool(const char* name) const = 0;
        virtual bool GetBool(const char* name, bool* result) const = 0;

        virtual bool SetUint(const char* name, unsigned value) = 0;
        virtual bool IsUint(const char* name) const = 0;
        virtual bool GetUint(const char* name, unsigned* result) const = 0;

        virtual bool SetInt(const char* name, int value) = 0;
        virtual bool IsInt(const char* name) const = 0;
        virtual bool GetInt(const char* name, int* result) const = 0;

        virtual bool SetUint64(const char* name, uint64_t value) = 0;
        virtual bool IsUint64(const char* name) const = 0;
        virtual bool GetUint64(const char* name, uint64_t* result) const = 0;

        virtual bool SetInt64(const char* name, int64_t value) = 0;
        virtual bool IsInt64(const char* name) const = 0;
        virtual bool GetInt64(const char* name, int64_t* result) const = 0;

        virtual bool SetDouble(const char* name, double value) = 0;
        virtual bool IsDouble(const char* name) const = 0;
        virtual bool GetDouble(const char* name, double* result) const = 0;

        virtual bool SetString(const char* name, const char* value, std::string::size_type size) = 0;
        virtual bool IsString(const char* name) const = 0;
        virtual bool GetStringLength(const char* name, std::string::size_type* size) const = 0;
        virtual bool GetString(const char* name, const char** value);

        virtual bool SetNull(const char* name) = 0;
        virtual bool IsNull(const char* name) const = 0;

        virtual bool OpenArray(const char* name) = 0;
        virtual bool ReserveArray(std::string::size_type size) = 0;
        virtual bool CloseArray() = 0;
    };
       

} // namespace Serializer


#endif // !SERIALIZER_HPP