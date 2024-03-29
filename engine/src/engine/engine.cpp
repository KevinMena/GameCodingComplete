
#include "engine/engine.hpp"

#include <utility>

#include "file_load_system/file_load_system.hpp"
#include "locale_handling/locale_handling.hpp"
#include "utils/engine_assert.hpp"
#include "utils/logger.hpp"


namespace Engine {

bool Engine::Init(const char *studio_name, const char *game_name) {
  LocaleHandling::SetStartupLocale();

  if (FileLoadSystem::SetupFileLoadSystem(studio_name, game_name)) {
    return false;
  }

  if (!Logging::Logger::SetupLogging(FileLoadSystem::GetTempDirectory())) {
    return false;
  }

  debug_assert::engine_handler::Setup(std::move(Logging::Logger::GetDefaultErrorLogger()));

  LOG(Logging::Logger::GetDefaultLogger(), 0, "Engine Started");
  return true;
}

Engine::~Engine() {
  LOG(Logging::Logger::GetDefaultLogger(), 0, "Engine Stopped");
  Logging::Logger::FinishLogging();
}
} // namespace Engine
