
#include "locale_handling/locale_handling.hpp"

#include "os_detection/os_detection.hpp"

#include "rapidjson/encodings.h"
#include "rapidjson/stringbuffer.h"

#include <locale>
#include <string>

namespace LocaleHandling {

const std::locale kStartupLocale = std::locale(kStartupLocaleName);

#if IS_WIN

UTF16_Container TranscodeToUTF16(const char *str) {
  // From http://rapidjson.org/md_doc_encoding.html
  rapidjson::StringStream source(str);
  UTF16_Container target;
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

UTF8_Container TranscodeToUTF8(const wchar_t *str) {
  // From http://rapidjson.org/md_doc_encoding.html
  rapidjson::GenericStringStream<rapidjson::UTF16<>> source(str);
  UTF8_Container target;
  bool hasError = false;
  while (source.Peek() != '\0') {
    if (!rapidjson::Transcoder<rapidjson::UTF16<>,
                               rapidjson::UTF8<>>::Transcode(source, target)) {
      hasError = true;
      break;
    }
  }
  if (!hasError) {
    return target;
  }
  return {};
}

#endif // IS_WIN

} // namespace LocaleHandling