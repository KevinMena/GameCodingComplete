
#include "serialization/json_serializer.hpp"

#include "serialization/serializer.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include <vector>
#include <functional>
#include <cstring>

namespace Serializer
{
    bool JsonSerializer::ParseText(const char *text) {
        return document.Parse(text).HasParseError();
    }

    bool JsonSerializer::Clear() {
        document.Clear();
        currentEntry.clear();
        currentDepth = 0;
        currentArray.clear();
        currentArrayIter.clear();
        buffer.Clear();
        return true;
    }

    bool JsonSerializer::Compile() {
        buffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        return document.Accept(writer);
    }

    bool JsonSerializer::CompilePretty() {
        buffer.Clear();
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        return document.Accept(writer);
    }

    bool JsonSerializer::GetSize(s_size *size) {
        *size = static_cast<s_size>(buffer.GetSize());
        return true;
    }

    bool JsonSerializer::GetLength(s_size *length) {
        *length = static_cast<s_size>(buffer.GetLength());
        return true;
    }

    bool JsonSerializer::GetText(char *text) {
        std::memcpy(text, buffer.GetString(), static_cast<size_t>(buffer.GetSize()));
        return true;
    }
}
