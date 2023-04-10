//
// Created by Jeromy Zhang on 4/9/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_

#include <optional>
#include <string>

namespace EngineCore::Utils {

std::optional<std::string_view> GetFileExtension(std::string_view path);
std::optional<std::string_view> GetFileName(std::string_view path);
constexpr auto isIn = [](auto &&k, auto &&...args) -> bool {
  return ((args == k) || ...);
};

}  // namespace EngineCore::Utils

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_UTILS_H_