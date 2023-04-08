//
// Created by Jeromy Zhang on 4/7/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_

#include <string>
#include <SDL.h>

namespace CSPill::EngineCore {

/**
 * \brief UI class for editor.
 */
class UI {
 public:
  UI(std::string title, int width, int height);
  [[nodiscard]] const std::string &GetTitle() const;
  void SetTitle(const std::string &title);
  [[nodiscard]] int GetWidth() const;
  void SetWidth(int width);
  [[nodiscard]] int GetHeight() const;
  void SetHeight(int height);
  [[nodiscard]] bool IsDisplay() const;
  void SetDisplay(bool display);
  virtual void Render(SDL_Renderer *renderer) = 0;

 private:
  std::string title_;
  int width_;
  int height_;
  bool display_ = true;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_
