
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
    JsonSerializer::JsonSerializer() {
        Clear();
    }

    inline bool JsonSerializer::IsInsideArray() const {
        return !m_currentArray.empty() && m_currentDepth == m_currentArray.back();
    }

    bool JsonSerializer::ParseText(const char *text) {
        return m_document.Parse(text).HasParseError();
    }

    bool JsonSerializer::Clear() {
        m_document.Clear();
        m_currentEntry.clear();
        m_currentDepth = 0;
        m_currentArray.clear();
        m_currentArrayIter.clear();
        m_buffer.Clear();
        m_currentEntry.push_back(m_document); // push root value
        return true;
    }

    bool JsonSerializer::Compile() {
        m_buffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(m_buffer);
        return m_document.Accept(writer);
    }

    bool JsonSerializer::CompilePretty() {
        m_buffer.Clear();
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(m_buffer);
        return m_document.Accept(writer);
    }

    bool JsonSerializer::GetSize(s_size *size) {
        *size = static_cast<s_size>(m_buffer.GetSize());
        return true;
    }

    bool JsonSerializer::GetLength(s_size *length) {
        *length = static_cast<s_size>(m_buffer.GetLength());
        return true;
    }

    bool JsonSerializer::GetText(char *text) {
        std::memcpy(text, m_buffer.GetString(), static_cast<size_t>(m_buffer.GetSize()));
        return true;
    }

    bool JsonSerializer::SetEntry(const char *name, s_size name_length, s_size version) {
        
        rapidjson::Value object(rapidjson::kObjectType);
        rapidjson::Document::AllocatorType& allocator = m_document.GetAllocator();

        rapidjson::Value key(kVersionEntryName, kVersionEntryNameLength, allocator); // copy string name
        rapidjson::Value val(version); 

        object.AddMember(key.Move(), val.Move(), allocator);
        
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& current = m_currentEntry.back().get();
            current.PushBack(object.Move(), allocator);

            // This is dumb but the library doesn't support other way
            m_currentEntry.push_back(current[current.Size()-1]);
        }
        // We are in a normal entry
        else 
        {
            rapidjson::Value nameKey(
                name, 
                static_cast<rapidjson::SizeType>(name_length), 
                allocator
            ); // copy string name

            rapidjson::Value& current = m_currentEntry.back().get();

            current.AddMember(nameKey.Move(), object.Move(), allocator);

            // This is dumb but the library doesn't support other way
            m_currentEntry.push_back(current[name]);
        }

        ++m_currentDepth;
        return true;
    }

    bool JsonSerializer::OpenEntry(const char *name, s_size *version) const {
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& new_current = *m_currentArrayIter.back();

            rapidjson::Value::MemberIterator iter = new_current.FindMember(kVersionEntryName);

            if (iter == new_current.MemberEnd())
            {
                return false;
            }

            *version = iter->value.Get<s_size>();

            m_currentEntry.push_back(new_current);
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

            rapidjson::Value& new_current = iter->value;

            iter = new_current.FindMember(kVersionEntryName);

            if (iter == new_current.MemberEnd())
            {
                return false;
            }

            *version = iter->value.Get<s_size>();

            m_currentEntry.push_back(new_current);
        }

        ++m_currentDepth;
        return true;
    }

    bool JsonSerializer::Close() const {
        m_currentEntry.pop_back();

        --m_currentDepth;
        return true;
    }

    bool JsonSerializer::SetBool(const char *name, s_size name_length, bool value) {
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

    bool JsonSerializer::IsBool(const char *name) const {
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& current = *m_currentArrayIter.back();

            return current.IsBool();
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

            return iter->value.IsBool();
        }
    }

    bool JsonSerializer::GetBool(const char *name, bool *result) const {
        // We are inside an array
        if (IsInsideArray())
        {
            rapidjson::Value& current = *m_currentArrayIter.back();

            *result = current.Get<bool>();
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

            *result = iter->value.Get<bool>();
        }

        return true;
    }
}
