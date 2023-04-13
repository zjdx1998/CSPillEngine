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

std::unique_ptr<Engine> Engine::Create(std::string_view title, int x, int y,
                                       int w, int h, Uint32 sdl_init_flags,
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

}  // namespace CSPill::EngineCore
