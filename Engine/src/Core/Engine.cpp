//
// Created by Jeromy Zhang on 4/11/23.
//

#include "Engine.h"

#include "ResourceManager.h"

namespace CSPill::EngineCore {

SDLWindow::SDLWindow(std::string_view title, int x, int y, int w, int h,
                     SDL_WindowFlags window_flags)
    : window_(SDL_CreateWindow(title.data(), x, y, w, h, window_flags),
              &SDL_DestroyWindow) {}

SDL_Window *SDLWindow::GetWindow() const { return window_.get(); }

SDLRenderer::SDLRenderer(SDL_Window *window, SDL_RendererFlags renderer_flags)
    : renderer_(SDL_CreateRenderer(window, -1, renderer_flags),
                &SDL_DestroyRenderer) {}

SDL_Renderer *SDLRenderer::GetRenderer() const { return renderer_.get(); }

Engine::Engine(std::unique_ptr<SDLWindow> window,
               std::unique_ptr<SDLRenderer> renderer)
    : window_(std::move(window)), renderer_(std::move(renderer)) {}

Engine::~Engine() { SDL_Quit(); }

SDL_Window *Engine::GetWindow() const { return window_->GetWindow(); }

SDL_Renderer *Engine::GetRenderer() const { return renderer_->GetRenderer(); }

std::unique_ptr<Engine> Engine::Create(std::string_view title, int w, int h,
                                       int x, int y, Uint32 sdl_init_flags,
                                       SDL_WindowFlags window_flags,
                                       SDL_RendererFlags renderer_flags) {
  if (SDL_Init(sdl_init_flags) < 0) {
    SDL_Log("Initialize SDL Error: %s!", SDL_GetError());
    return nullptr;
  }

  auto window = std::make_unique<SDLWindow>(title, x, y, w, h, window_flags);
  if (window->GetWindow() == nullptr) {
    return nullptr;
  }

  auto renderer =
      std::make_unique<SDLRenderer>(window->GetWindow(), renderer_flags);
  if (renderer->GetRenderer() == nullptr) {
    return nullptr;
  }
  ResourceManager::GetInstance().SetRenderer(renderer->GetRenderer());
  return std::unique_ptr<Engine>(
      new Engine(std::move(window), std::move(renderer)));
}
bool Engine::AddObject(const std::string &name, GameObject &&object) {
  if (objects_.find(name) != objects_.end()) {
    return false;
  }
  objects_[name] = std::make_unique<GameObject>(std::move(object));
  return true;
}

GameObject *Engine::GetObject(const std::string &name) {
  if (objects_.find(name) == objects_.end()) return nullptr;
  return objects_.at(name).get();
}

void Engine::SetGameOver(bool game_over) { this->game_over_ = game_over; }
[[nodiscard]] bool Engine::IsGameOver() const { return this->game_over_; }

void Engine::Run(int FPS) {
  while (!Engine::IsGameOver()) {
    for (const auto &obj : objects_) {
      obj.second->Update();
      obj.second->Render(renderer_->GetRenderer());
    }
  }
}

}  // namespace CSPill::EngineCore
