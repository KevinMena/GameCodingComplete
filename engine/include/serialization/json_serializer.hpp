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
    class JsonSerializer final : ISerializer
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
        virtual bool SetEntry(const char *name, s_size name_length, s_size version) final;
        /*
        Gets in the space of an entry and return its version
        If it is at the same level as an opened array, the name is not used and the
        array entry is opened
        */
        virtual bool OpenEntry(const char *name, s_size *version) const final;
        /* Close an entry */
        virtual bool Close() const final;

    private:
        /* Current Entry we are looking at. Used as a Stack */
        mutable std::vector<std::reference_wrapper<rapidjson::Value> > m_currentEntry;

        /* Current Depth we are looking at */
        mutable s_size m_currentDepth;
        
        /* Current Array Depth. Used as a Stack */
        mutable std::vector<s_size> m_currentArray;

        /* Current Iterator of the Current Array we are looking at. Used as a Stack */
        mutable std::vector<rapidjson::Value::ConstValueIterator> m_currentArrayIter;

        /* Where we compile */
        rapidjson::StringBuffer m_buffer;

        /* Where we store the JSON structure */
        rapidjson::Document m_document;

        /* The name of the version entry */
        static constexpr char  kVersionEntryName [] = "__VERSION__";

        /* Length of the name of the version entry, excluding null terminator */
        static constexpr s_size kVersionEntryNameLength = sizeof(kVersionEntryName)-1;
    };
} // namespace Serializer


#endif // !JSON_SERIALIZER_HPP