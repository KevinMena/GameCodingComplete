#pragma once
#ifndef JSON_SERIALIZER_HPP
#define JSON_SERIALIZER_HPP 1

#include "serialization/serializer.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <vector>
#include <functional>

namespace Serializer
{
    class JsonSerializer final : public ISerializer
    {
    public:
        JsonSerializer();
        JsonSerializer(JsonSerializer &&) = default;
        /* Copy is not allowed because it doesn't make sense */
        JsonSerializer(const JsonSerializer &) = delete;
        JsonSerializer &operator=(JsonSerializer &&) = default;
        /* Copy is not allowed because it doesn't make sense */
        JsonSerializer &operator=(const JsonSerializer &) = delete;
        ~JsonSerializer() = default;

        /* Parse from Text */
        bool ParseText(const char *text) final;
        /* Clear internals */
        bool Clear() final;
        /* Compile internals */
        bool Compile() final;
        /* Compile internals in a Pretty Format*/
        bool CompilePretty() final;
        /* Get Size in bytes of the compiled internals */
        bool GetSize(s_size *size) final;
        /* Get Length (amount of chars used) of the compiled internals */
        bool GetLength(s_size *length) final;
        /* Put the compiled internals in a char array. It should have at least enough space */
        bool GetText(char *text) final;
        
    
        /*
        Open an space for a new entry with name and version. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and the
        entry is appended to the array
        */
        bool SetEntry(const char *name, s_size name_length, s_size version) final;
        /*
        Gets in the space of an entry and return its version
        If it is at the same level as an opened array, the name is not used and the
        array entry is opened
        */
        bool OpenEntry(const char *name, s_size *version) const final;
        /* Close an entry */
        bool CloseEntry() const final;

        /*
        Sets a new bool. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetBool(const char *name, s_size name_length, bool value) final;
        /*
        Gets if the current/named entry is a bool
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsBool(const char *name) const final;
        /*
        Gets the named or current bool entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetBool(const char *name, bool *result) const final;

        /*
        Sets a new unsigned entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetUint(const char *name, s_size name_length, unsigned value) final;
        /*
        Gets if the current/named entry is an unsigned
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsUint(const char *name) const final;
        /*
        Gets the named or current unsigned entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetUint(const char *name, unsigned *result) const final;

        /*
        Sets a new int entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetInt(const char *name, s_size name_length, int value) final;
        /*
        Gets if the current/named entry is an int
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsInt(const char *name) const final;
        /*
        Gets the named or current int entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetInt(const char *name, int *result) const final;

        /*
        Sets a new uint64_t entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetUint64(const char *name, s_size name_length, uint64_t value) final;
        /*
        Gets if the current/named entry is an uint64_t
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsUint64(const char *name) const final;
        /*
        Gets the named or current uint64_t entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetUint64(const char *name, uint64_t *result) const final;

        /*
        Sets a new int64_t entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetInt64(const char *name, s_size name_length, int64_t value) final;
        /*
        Gets if the current/named entry is an int64_t
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsInt64(const char *name) const final;
        /*
        Gets the named or current int64_t entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetInt64(const char *name, int64_t *result) const final;

        /*
        Sets a new double entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetDouble(const char *name, s_size name_length, double value) final;
        /*
        Gets if the current/named entry is an double
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsDouble(const char *name) const final;
        /*
        Gets the named or current double entry
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetDouble(const char *name, double *result) const final;

        /*
        Sets a new string entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetString(const char *name, s_size name_length, const char *value, s_size length) final;
        /*
        Gets if the current/named entry is a string
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsString(const char *name) const final;
        /*
        Gets the named or current string entry length (UTF makes this tricky)
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetStringLength(const char *name, s_size *result) const final;
        /*
        Gets the named or current string size in bytes
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetStringSize(const char *name, s_size *result) const final;
        /*
        Gets the named or current string entry. The result pointer must be able to hold the string
        If it is at the same level as an opened array, the name is not used and it
        gets the current opened entry is
        */
        bool GetString(const char *name, char *result) const final;

        /*
        Sets a new null entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetNull(const char *name, s_size name_length) final;
        /*
        Gets if the current/named entry is null
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsNull(const char *name) const final;

        /*
        Sets a new array entry. Length excludes null terminator.
        If it is at the same level as an opened array, the name is not used and it
        is appended to the array
        */
        bool SetArray(const char *name, s_size name_length) final;
        /*
        Reserve Array memory, might be hard to predict due to recursions
        */
        bool ReserveArray(const char *name, s_size size) final;
        /*
        Gets if the current/named entry is an array
        If it is at the same level as an opened array, the name is not used and it
        checks if the current opened entry is
        */
        bool IsArray(const char *name) const final;
        /*
        Gets in the space of an entry
        If it is at the same level as an opened array, the name is not used and the
        array entry is opened
        */
        bool OpenArray(const char *name) const final;
        /* Get Current/Named Array Capacity */
        bool GetArrayCapacity(const char *name, s_size *size) const final;
        /* Moves the Current Array Iterator. It starts at begin(). This function will
        * return false when it hits end() */
        bool MoveArray() const final;
        /* Close an array entry */
        bool CloseArray() const final;

    private:

        /* Check if we are currently inside an array */
        bool IsInsideArray() const;

        /* Sets an entry of type T (Only primitives and no pointers)*/
        template<typename T>
        bool SetType(const char *name, s_size name_length, T value);

        /* If an entry is of type T (Only primitives and no pointers)*/
        template<typename T>
        bool IsType(const char *name) const;

        /* Get an entry of type T (Only primitives and no pointers)*/
        template<typename T>
        bool GetType(const char *name, T *result) const;

        /* Current Entry we are looking at. Used as a Stack */
        mutable std::vector<std::reference_wrapper<rapidjson::Value> > m_currentEntry;

        /* Current Depth we are looking at */
        mutable s_size m_currentDepth;
        
        /* Current Array Depth. Used as a Stack */
        mutable std::vector<s_size> m_currentArray;

        /* Current Iterator of the Current Array we are looking at. Used as a Stack */
        mutable std::vector<rapidjson::Value::ValueIterator> m_currentArrayIter;

        /* Where we compile */
        rapidjson::StringBuffer m_buffer;

        /* Where we store the JSON structure */
        rapidjson::Document m_document;

        /* The name of the version entry */
        static constexpr char  kVersionEntryName [] = "__VERSION__";

        /* Length of the name of the version entry, excluding null terminator */
        static constexpr s_size kVersionEntryNameLength = sizeof(kVersionEntryName)-1;

        /* Character type used */
        using CharType = typename rapidjson::Document::Ch; 
    };

    inline bool JsonSerializer::IsInsideArray() const {
        return !m_currentArray.empty() && m_currentDepth == m_currentArray.back();
    }

    template<typename T>
    bool JsonSerializer::SetType(const char *name, s_size name_length, T value) {
        rapidjson::Document::AllocatorType& allocator = m_document.GetAllocator();
        rapidjson::Value val(value); 

        rapidjson::Value& current = m_currentEntry.back().get();
        
        // We are inside an array
        if (IsInsideArray())
        {
            current.PushBack(val.Move(), allocator);
        }
        // We are in a normal entry
        else 
        {
            rapidjson::Value nameKey(
                name, 
                static_cast<rapidjson::SizeType>(name_length), 
                allocator
            ); // copy string name

            current.AddMember(nameKey.Move(), val.Move(), allocator);
        }

        return true;
    }

    template<typename T>
    bool JsonSerializer::IsType(const char *name) const {
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& current = *m_currentArrayIter.back();

            return current.Is<T>();
        }
        // We are in a normal entry
        else 
        {
            rapidjson::Value& current = m_currentEntry.back().get();

            rapidjson::Value::MemberIterator iter = current.FindMember(name);

            if (iter == current.MemberEnd())
            {
                return false;
            }

            return iter->value.Is<T>();
        }
    }

    template<typename T>
    bool JsonSerializer::GetType(const char *name, T *result) const {
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& current = *m_currentArrayIter.back();

            *result = current.Get<T>();
        }
        // We are in a normal entry
        else 
        {
            rapidjson::Value& current = m_currentEntry.back().get();

            rapidjson::Value::MemberIterator iter = current.FindMember(name);

            if (iter == current.MemberEnd())
            {
                return false;
            }

            *result = iter->value.Get<T>();
        }

        return true;
    }

    inline bool JsonSerializer::SetBool(const char *name, s_size name_length, bool value) {
        return SetType<bool>(name, name_length, value);
    }

    inline bool JsonSerializer::IsBool(const char *name) const {
        return IsType<bool>(name);
    }

    inline bool JsonSerializer::GetBool(const char *name, bool *result) const {
        return GetType<bool>(name, result);
    }

    inline bool JsonSerializer::SetUint(const char *name, s_size name_length, unsigned value) {
        return SetType<unsigned>(name, name_length, value);
    }

    inline bool JsonSerializer::IsUint(const char *name) const {
        return IsType<unsigned>(name);
    }
    
    inline bool JsonSerializer::GetUint(const char *name, unsigned *result) const {
        return GetType<unsigned>(name, result);
    }

    inline bool JsonSerializer::SetInt(const char *name, s_size name_length, int value) {
        return SetType<int>(name, name_length, value);
    }

    inline bool JsonSerializer::IsInt(const char *name) const {
        return IsType<int>(name);
    }
    
    inline bool JsonSerializer::GetInt(const char *name, int *result) const {
        return GetType<int>(name, result);
    }

    inline bool JsonSerializer::SetUint64(const char *name, s_size name_length, uint64_t value) {
        return SetType<uint64_t>(name, name_length, value);
    }

    inline bool JsonSerializer::IsUint64(const char *name) const {
        return IsType<uint64_t>(name);
    }
    
    inline bool JsonSerializer::GetUint64(const char *name, uint64_t *result) const {
        return GetType<uint64_t>(name, result);
    }

    inline bool JsonSerializer::SetInt64(const char *name, s_size name_length, int64_t value) {
        return SetType<int64_t>(name, name_length, value);
    }

    inline bool JsonSerializer::IsInt64(const char *name) const {
        return IsType<int64_t>(name);
    }
    
    inline bool JsonSerializer::GetInt64(const char *name, int64_t *result) const {
        return GetType<int64_t>(name, result);
    }

    inline bool JsonSerializer::SetDouble(const char *name, s_size name_length, double value) {
        return SetType<double>(name, name_length, value);
    }

    inline bool JsonSerializer::IsDouble(const char *name) const {
        return IsType<double>(name);
    }
    
    inline bool JsonSerializer::GetDouble(const char *name, double *result) const {
        return GetType<double>(name, result);
    }

    inline bool JsonSerializer::IsString(const char *name) const {
        return IsType<const typename CharType*>(name);
    }

} // namespace Serializer


#endif // !JSON_SERIALIZER_HPP