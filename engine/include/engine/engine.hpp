#pragma once
#ifndef ENGINE_HPP
#define ENGINE_HPP 1

/* All things related to engine startup and stop */
namespace Engine {

class Engine {
public:
  Engine() = default;
  Engine(Engine &&) = default;
  Engine(const Engine &) = delete;
  Engine &operator=(Engine &&) = default;
  Engine &operator=(const Engine &) = delete;
  ~Engine();

  /* All the engine Setups and Startup. Returns true if successful */
  bool Init(const char *studio_name, const char *game_name);
};

} // namespace Engine

#endif // !ENGINE_HPP