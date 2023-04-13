//
// Created by Jeromy Zhang on 4/9/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_

#include <optional>
#include <string>
#include <memory>

#include "SDL.h"

namespace EngineCore::Utils {

std::optional<std::string_view> GetFileExtension(std::string_view path);
std::optional<std::string_view> GetFileName(std::string_view path);
constexpr auto isIn = [](auto &&k, auto &&...args) -> bool {
  return ((args == k) || ...);
};

/**
 * Convert data in layer into row and column.
 * @param data data ranges from 00000000 to 99999999.
 * @return a pair of coordinates.
 */
std::pair<int, int> GetRowAndCol(int data);

/**
 * Reverse of GetRowAndCol, converts a pair of coordinates into data.
 * @param row_and_col pair of coordinates (U,V).
 * @return the raw data stored in scene.
 */
int GetDataFromRowAndCol(const std::pair<int, int> &row_and_col);

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> CropTexture(
    SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect &src_rect);

}  // namespace EngineCore::Utils

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_
