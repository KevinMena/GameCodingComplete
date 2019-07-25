
#include "locale_handling/locale_handling.hpp"

#include "os_detection/os_detection.hpp"

#include "rapidjson/encodings.h"
#include "rapidjson/stringbuffer.h"

#include <locale>
#include <string>

namespace LocaleHandling {

const std::locale kStartupLocale = std::locale(kStartupLocaleName);

#if IS_WIN

rapidjson::GenericStringBuffer<rapidjson::UTF16<>> Transcode(const char *str) {
  // From http://rapidjson.org/md_doc_encoding.html
  rapidjson::StringStream source(str);
  rapidjson::GenericStringBuffer<rapidjson::UTF16<>> target;
  bool hasError = false;
  while (source.Peek() != '\0') {
    if (!rapidjson::Transcoder<rapidjson::UTF8<>,
                               rapidjson::UTF16<>>::Transcode(source, target)) {
      hasError = true;
      break;
    }
  }
  if (!hasError) {
    return target;
  }
  return {};
}

inline rapidjson::GenericStringBuffer<rapidjson::UTF16<>>
Transcode(const std::string &str) {
  return Transcode(str.c_str());
}

#endif // IS_WIN

} // namespace LocaleHandling