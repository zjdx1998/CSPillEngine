//
// Created by Jeromy Zhang on 4/7/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_

#include <SDL.h>

#include <string>

namespace CSPill::EngineCore {

/**
 * \brief UI class for editor.
 *
 * Generate UI object that the editor is going to display. The UI object
 * contains dimensions of the UI and if it is required to be displayed.
 */
class UI {
 public:
  /**
   * Constructor of UI.
   * @param title a string of the title of the UI
   * @param width int, the width of the UI
   * @param height int, the height of the UI
   */
  UI(std::string title, int width, int height);

  /**
   * Get the title of the UI.
   * @return a string of the title of the UI
   */
  [[nodiscard]] const std::string &GetTitle() const;

  /**
   * Set the member variable of title to title.
   * @param title a string of the requested title
   */
  void SetTitle(const std::string &title);

  /**
   * Get the width of the UI.
   * @return int, the width of the UI
   */
  [[nodiscard]] int GetWidth() const;

  /**
   * Set the width to width.
   * @param width int, the target UI width
   */
  void SetWidth(int width);

  /**
   * Get the height of the UI.
   * @return int, the height of the UI
   */
  [[nodiscard]] int GetHeight() const;

  /**
   * Set the height to height.
   * @param height int, the target UI height
   */
  void SetHeight(int height);

  /**
   * Get if it is displayed.
   * @return true if the UI is displayed, false otherwise
   */
  [[nodiscard]] bool IsDisplay() const;

  /**
   * Set to display.
   * @param height bool, true if the UI is displayed, false otherwise
   */
  void SetDisplay(bool display);

  /**
   * Pure virtual function to render the current UI.
   * @param renderer SDL_Renderer that is going to be used
   */
  virtual void Render(SDL_Renderer *renderer) = 0;

 private:
  std::string title_;
  int width_;
  int height_;
  bool display_ = true;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_UI_H_
