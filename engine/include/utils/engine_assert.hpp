#pragma once
#ifndef ENGINE_ASSERT_HPP
#define ENGINE_ASSERT_HPP 1

#include "debug_assert.hpp"

#include <memory>
#include <utility>

#include "utils/logger.hpp"

namespace debug_assert {

/*
  The default handler that writes a message a Logger.
  Inherit from it in your module handler.
  Following:
  https://github.com/foonathan/debug_assert/blob/master/debug_assert.hpp
*/
struct engine_handler {

  /* Setup Logger for asserts */
  static void Setup(const std::shared_ptr<Logging::Logger> &logger) {
    s_errorLogger = logger;
  }

  /* Setup Logger for asserts */
  static void Setup(std::shared_ptr<Logging::Logger> &&logger) {
    s_errorLogger = std::move(logger);
  }

  /* Logger Level used for assertions */
  constexpr static int s_loggerLevel = 0;

  /*
  \effects Prints a message to a Logger.
  \notes It can optionally accept an additional message string.
  */
  static void handle(const source_location &loc, const char *expression,
                     const char *message = nullptr) noexcept {
    if (*expression == '\0') {
      if (message) {
        s_errorLogger->Log(loc.file_name, loc.line_number, s_loggerLevel,
                           "[debug assert] Unreachable code reached - %s.\n",
                           message);
      } else {
        s_errorLogger->Log(loc.file_name, loc.line_number, s_loggerLevel,
                           "[debug assert] Unreachable code reached.\n");
      }
    } else if (message) {
      s_errorLogger->Log(loc.file_name, loc.line_number, s_loggerLevel,
                         "[debug assert] Assertion '%s' failed - %s.\n",
                         expression, message);
    } else {
      s_errorLogger->Log(loc.file_name, loc.line_number, s_loggerLevel,
                         "[debug assert] Assertion '%s' failed.\n", expression);
    }
  }
  private:
  /* Logger to use */
  static std::shared_ptr<Logging::Logger> s_errorLogger;
};

/* 
  Default Engine Asserts Handler
  Complete Explanation: https://github.com/foonathan/debug_assert
  Example: DEBUG_ASSERT(1 + 1 == 2, debug_assert::engine_module{}); // basic  
 */
struct engine_module
    : engine_handler, // use the engine handler
      set_level<-1> // level -1, i.e. all assertions, 0 would mean none, 1 would
                    // be level 1, 2 level 2 or lower,...
{};

} // namespace debug_assert

#endif // !ENGINE_ASSERT_HPP