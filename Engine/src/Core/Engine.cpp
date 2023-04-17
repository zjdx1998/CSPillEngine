//
// Created by Jeromy Zhang on 4/11/23.
//

#include "Engine.h"

#include "CameraComponent.h"
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
               std::unique_ptr<SDLRenderer> renderer, int width, int height)
    : window_(std::move(window)),
      renderer_(std::move(renderer)),
      width_(width),
      height_(height) {}

Engine::~Engine() { SDL_Quit(); }

std::pair<int, int> Engine::GetWindowSize() const { return {width_, height_}; }

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
      new Engine(std::move(window), std::move(renderer), w, h));
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
bool Engine::IsGameOver() const { return this->game_over_; }

void Engine::SwitchScene(Scene *scene) { this->scene_ = scene; }

void Engine::RefreshScene() {
  SDL_SetRenderDrawColor(renderer_->GetRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(renderer_->GetRenderer());
  SDL_Texture *level = scene_->Render(renderer_->GetRenderer());
  if (level) {
    SDL_Rect dstRect = {0, 0, GetWindowSize().first, GetWindowSize().second};
    if (auto camera = GetObject("Camera")) {
      auto camera_component =
          (CameraComponent *)(camera->GetComponent("CameraComponent"));
      auto &viewport = camera_component->GetViewport();
      dstRect.x = std::max(dstRect.x, (int)viewport.x);
      dstRect.y = std::max(dstRect.y, (int)viewport.y);
      dstRect.w = std::min(dstRect.w, (int)viewport.w);
      dstRect.h = std::min(dstRect.h, (int)viewport.h);
    }
    SDL_RenderCopy(renderer_->GetRenderer(), level, &dstRect, nullptr);
  }
}

void Engine::Run(int FPS) {
  float dt = 0;
  while (!Engine::IsGameOver()) {
    auto start_time = std::chrono::high_resolution_clock::now();
    RefreshScene();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) game_over_ = true;
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(GetWindow()))
        game_over_ = true;
      if (event.type == SDL_KEYDOWN) {
        key_pressed_.insert(event.key.keysym.sym);
      }
      if (event.type == SDL_KEYUP) {
        key_pressed_.erase(key_pressed_.find(event.key.keysym.sym));
      }
    }
    for (const auto &obj : objects_) {
      obj.second->Update(dt);
      obj.second->Render(renderer_->GetRenderer());
    }
    SDL_RenderPresent(renderer_->GetRenderer());
    auto end_time = std::chrono::high_resolution_clock::now();
    dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
             end_time - start_time)
             .count();
    if (dt < 1000.0 / FPS) {
      SDL_Delay(1000.0 / FPS - dt);
    }
  }
}
bool Engine::IsKeyPressed(const std::string &key) {
  return key_pressed_.find(SDL_GetKeyFromName(key.data())) !=
         key_pressed_.end();
}

}  // namespace CSPill::EngineCore
