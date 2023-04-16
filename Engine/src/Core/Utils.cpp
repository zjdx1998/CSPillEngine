//
// Created by Jeromy Zhang on 4/9/23.
//

#include "Utils.h"

#include <filesystem>
#include <memory>

#include "SDL.h"

namespace EngineCore::Utils {

namespace fs = std::filesystem;

std::optional<std::string_view> GetFileExtension(std::string_view path) {
  fs::path file_path = path;
  if (fs::exists(file_path)) {
    return file_path.extension().string();
  }
  return std::nullopt;
}

std::optional<std::string_view> GetFileName(std::string_view path) {
  fs::path file_path = path;
  if (fs::exists(file_path)) {
    return file_path.filename().string();
  }
  return std::nullopt;
}

std::pair<int, int> GetRowAndCol(int data) {
  return {data / 1000, data % 1000};
}

int GetDataFromRowAndCol(const std::pair<int, int> &row_and_col) {
  return row_and_col.first * 1000 + row_and_col.second;
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> CropTexture(
    SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect &src_rect) {
  auto cropped = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, src_rect.w, src_rect.h),
      [](SDL_Texture *t) { SDL_DestroyTexture(t); });
  SDL_SetRenderTarget(renderer, cropped.get());
  SDL_SetTextureBlendMode(cropped.get(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, &src_rect, nullptr);
  SDL_SetRenderTarget(renderer, nullptr);
  return std::move(cropped);
}

}  // namespace EngineCore::Utils