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
        JsonSerializer() = default;
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
        
    
    private:
        /* Current Entry we are looking at. Used as a Stack */
        mutable std::vector<std::reference_wrapper<rapidjson::Value> > currentEntry;

        /* Current Depth we are looking at */
        mutable s_size currentDepth;
        
        /* Current Array Depth. Used as a Stack */
        mutable std::vector<s_size> currentArray;

        /* Current Iterator of the Current Array we are looking at. Used as a Stack */
        mutable std::vector<rapidjson::Value::ConstValueIterator> currentArrayIter;

        /* Where we compile */
        rapidjson::StringBuffer buffer;

        /* Where we store the JSON structure */
        rapidjson::Document document;

    };
} // namespace Serializer


#endif // !JSON_SERIALIZER_HPP