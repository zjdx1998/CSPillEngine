//
// Created by Jeromy Zhang on 4/9/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_

#include <memory>
#include <optional>
#include <string>

#include "SDL.h"

namespace EngineCore::Utils {

constexpr std::string_view TRANSFORM_COMPONENT = "TransformComponent";
constexpr std::string_view CAMERA_COMPONENT = "CameraComponent";
constexpr std::string_view COLLISION_COMPONENT = "CollisionComponent";
constexpr std::string_view ANIMATION_COMPONENT = "AnimationComponent";

/**
 * Get the extension of the file.
 * @param path a string of the path to the target file
 * @return a string, contains the extension of the file
 */
std::optional<std::string_view> GetFileExtension(std::string_view path);

/**
 * Get the name of the file.
 * @param path a string of the path to the target file
 * @return a string, contains the name of the file
 */
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

/**
 * Crop a texture.
 * @param renderer SDL_Renderer that is going to be used
 * @param texture SDL_Texture, the target texture
 * @param src_rect SDL_Rect, cropped area
 * @return unique_ptr of the cropped texture
 */
std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> CropTexture(
    SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect &src_rect);

}  // namespace EngineCore::Utils

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_
