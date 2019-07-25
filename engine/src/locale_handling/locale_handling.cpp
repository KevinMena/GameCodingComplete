
#include "locale_handling/locale_handling.hpp"

#include <locale>

namespace LocaleHandling {
const std::locale kStartupLocale = std::locale(kStartupLocaleName);
} // namespace LocaleHandling