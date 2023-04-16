//
// Created by Jeromy Zhang on 4/11/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_

#include <memory>
#include <string>

#include "GameObject.h"
#include "SDL.h"
#include "Scene.h"

namespace CSPill::EngineCore {
/**
 * \brief SDL_window wrapper class
 *
 * A helper class to create SDL_window object by the same arguments of
 * SDL_CreateWindow. SDL_window object can be acquired by GetWindow.
 */
class SDLWindow {
 public:
  /**
   * Constructor for SDLWindow. It takes same arguments as SDL_CreateWindow
   * @param title the title of the window
   * @param x the x position of the window
   * @param y the y position of the window
   * @param w the width of the window, in screen coordinates
   * @param h the height of the window, in screen coordinates
   * @param window_flags 0, or one or more SDL_WindowFlags OR'd together
   */
  SDLWindow(std::string_view title, int x, int y, int w, int h,
            SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

  /**
   * Destructor for SDLWindow.
   */
  ~SDLWindow() = default;

  /**
   * Getter function for SDL_window.
   * @return a pointer for SDL_window object
   */
  [[nodiscard]] SDL_Window *GetWindow() const;

 private:
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_;
};

/**
 * \brief SDL_renderer wrapper class
 *
 * A helper class to create SDL_renderer object by the same arguments of
 * SDL_CreateRenderer. SDL_renderer object can be acquired by GetRenderer.
 */
class SDLRenderer {
 public:
  /**
   * Constructor for SDLRenderer. It takes same arguments as SDL_CreateRenderer.
   * The index is set default to -1, which means it initialize the first one
   * supporting the requested flags
   * @param window the window where rendering is displayed
   * @param renderer_flags 0, or one or more SDL_RendererFlags OR'd together
   */
  explicit SDLRenderer(
      SDL_Window *window,
      SDL_RendererFlags renderer_flags = static_cast<SDL_RendererFlags>(
          SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));

  /**
   * Destructor of SDLRenderer.
   */
  ~SDLRenderer() = default;

  /**
   * Getter function for SDL_Renderer.
   * @return a pointer for SDL_Renderer object
   */
  [[nodiscard]] SDL_Renderer *GetRenderer() const;

 private:
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};

/**
 * \brief main Engine object
 *
 * The Engine class is the initializer for CSPillEngine. It initializes several
 * key functions and objects, such as SDL, SDL_window, SDL_renderer, and
 * ResourceManager.
 */
class Engine {
 public:
  Engine(Engine const &) = delete;

  void operator=(Engine const &) = delete;

  /**
   * Destructor of Engine.
   */
  ~Engine();

  /**
   * Getter function for SDL_window.
   * @return a pointer for SDL_window object
   */
  [[nodiscard]] SDL_Window *GetWindow() const;

  /**
   * Getter function for SDL_Renderer.
   * @return a pointer for SDL_Renderer object
   */
  [[nodiscard]] SDL_Renderer *GetRenderer() const;

  /**
   * Factory function to create Engine object.
   * @param title the title of the window
   * @param x the x position of the window
   * @param y the y position of the window
   * @param w the width of the window, in screen coordinates
   * @param h the height of the window, in screen coordinates
   * @param sdl_init_flags SDL_init subsystem initialization flags
   * @param window_flags 0, or one or more SDL_WindowFlags OR'd together
   * @param renderer_flags 0, or one or more SDL_RendererFlags OR'd together
   * @return unique_ptr for Engine object
   */
  static std::unique_ptr<Engine> Create(
      std::string_view title, int w, int h, int x = SDL_WINDOWPOS_CENTERED,
      int y = SDL_WINDOWPOS_CENTERED,
      Uint32 sdl_init_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER |
                              SDL_INIT_GAMECONTROLLER,
      SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
          SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI),
      SDL_RendererFlags renderer_flags = static_cast<SDL_RendererFlags>(
          SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));

  /**
   * Add a game object to engine.
   * @param name the name of the GameObject
   * @param object GameObject, reference to the object
   * @return true if the GameObject is added, otherwise false
   */
  bool AddObject(const std::string &name, GameObject &&object);

  /**
   * Get the GameObject by its name.
   * @param name a string of the name of the GameObject
   * @return a pointer to the GameObject
   */
  GameObject *GetObject(const std::string &name);

  /**
   * Set the game status to game_over.
   * @param game_over bool, the status of the game
   */
  void SetGameOver(bool game_over);

  /**
   * Get the status of the game.
   * @return bool, true if the game is over, false otherwise
   */
  [[nodiscard]] bool IsGameOver() const;

  /**
   * Switch the scene to target scene.
   * @param scene a pointer to the target scene
   */
  void SwitchScene(Scene *scene);

  /**
   * Get the window size.
   * @return a pair contains the window's width and height
   */
  std::pair<int, int> GetWindowSize() const;

  /**
   * Run the program by a certain FPS
   * @param FPS int, FPS cap for the loop, default to be 60
   */
  void Run(int FPS = 60);

 private:
  int width_, height_;

  Engine(std::unique_ptr<SDLWindow> window,
         std::unique_ptr<SDLRenderer> renderer, int width, int height);

  std::unique_ptr<SDLWindow> window_;
  std::unique_ptr<SDLRenderer> renderer_;
  Scene *scene_;
  std::unordered_map<std::string, std::unique_ptr<GameObject>> objects_;
  bool game_over_ = false;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_ENGINE_H_
