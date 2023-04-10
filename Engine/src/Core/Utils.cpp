//
// Created by Jeromy Zhang on 4/9/23.
//

#include "Utils.h"

#include <filesystem>

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

}  // namespace EngineCore::Utils