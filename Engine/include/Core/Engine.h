//
// Created by Jeromy Zhang on 4/11/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_

#include <memory>
#include <string>

#include "SDL.h"
#include "GameObject.h"

namespace CSPill::EngineCore {

class SDLWindow {
 public:
  SDLWindow(std::string_view title, int x, int y, int w, int h,
            SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));
  ~SDLWindow() = default;
  [[nodiscard]] SDL_Window *GetWindow() const;

 private:
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
};

class SDLRenderer {
 public:
  explicit SDLRenderer(
      SDL_Window *window,
      SDL_RendererFlags renderer_flags = static_cast<SDL_RendererFlags>(
          SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
  ~SDLRenderer() = default;
  [[nodiscard]] SDL_Renderer *GetRenderer() const;

 private:
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};

class Engine {
 public:
  Engine(Engine const &) = delete;
  void operator=(Engine const &) = delete;
  ~Engine();
  [[nodiscard]] SDL_Window *GetWindow() const;
  [[nodiscard]] SDL_Renderer *GetRenderer() const;
  static std::unique_ptr<Engine> Create(
      std::string_view title, int w, int h, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED,
      Uint32 sdl_init_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER |
          SDL_INIT_GAMECONTROLLER,
      SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
      SDL_RendererFlags renderer_flags = static_cast<SDL_RendererFlags>(
          SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));

  bool AddObject(const std::string &name, GameObject &&object);
  GameObject *GetObject(const std::string &name);

  void SetGameOver(bool game_over);
  [[nodiscard]] bool IsGameOver() const;

  void Run(int FPS = 60);
 private:
  Engine(std::unique_ptr<SDLWindow> window,
         std::unique_ptr<SDLRenderer> renderer);
  std::unique_ptr<SDLWindow> window_;
  std::unique_ptr<SDLRenderer> renderer_;
  std::unordered_map<std::string, std::unique_ptr<GameObject>> objects_;
  bool game_over_ = false;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_
