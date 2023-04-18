#ifndef CSPILLENGINE_ENGINECORE_UITEXT_H_
#define CSPILLENGINE_ENGINECORE_UITEXT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::UI {
/**
 * \brief Text UI
 *
 * UIText displays score and level of the game.
 */
class UIText : public EngineCore::GameObject {
 public:
  /**
   * Factory function to create UIText.
   * @param font_name_ a string of the name of the font
   * @param content a string of the content to be displayed
   * @param pos Vec2D, position of the UIText
   * @param size Vec2D, size of the UIText
   * @param font_size int, size of the font
   * @return
   */
  static std::unique_ptr<GameObject> Create(std::string_view font_name_,
                                            std::string_view content,
                                            const CSPill::Math::Vec2D &pos,
                                            const CSPill::Math::Vec2D &size,
                                            int font_size);

  UIText(UIText &&) = default;

  /**
   * Destructor of UIText
   */
  ~UIText() override;

  /**
   * Get the name of the font.
   * @return a string of name of the font.
   */
  [[nodiscard]] const std::string &GetFontName() const;

  /**
   * Set the font name.
   * @param font_name a string of name of the font.
   */
  void SetFontName(const std::string &font_name);

  /**
   * Get the font size.
   * @return int, font size
   */
  [[nodiscard]] int GetFontSize() const;

  /**
   * Set the font size.
   * @param font_size int, font size
   */
  void SetFontSize(int font_size);

  /**
   * Get display size.
   * @return Vec2D, represents the width and height font
   */
  [[nodiscard]] const CSPill::Math::Vec2D &GetSize() const;

  /**
   * Set the display size.
   * @param size Vec2D, represents the width and height font
   */
  void SetSize(const CSPill::Math::Vec2D &size);

  /**
   * Get the position of the font.
   * @return Vec2D, the position of the font
   */
  [[nodiscard]] const CSPill::Math::Vec2D &GetPos() const;

  /**
   * Set the position of the font.
   * @param pos Vec2D, the position of the font
   */
  void SetPos(const CSPill::Math::Vec2D &pos);

  /**
   * Get the content of the font.
   * @return a string of contents to be displayed
   */
  [[nodiscard]] const std::string &GetContent() const;

  /**
   * Set the displayed contents.
   * @param content a string of contents to be displayed
   */
  void SetContent(const std::string &content);

  /**
   * Render the contents.
   * @param renderer SDL_Renderer that is going to be used
   */
  void Render(SDL_Renderer *renderer) override;

  /**
   * Get the bounding box for the font.
   * @return SDL_FRect, the bounding box
   */
  SDL_FRect GetBoundingBox();

 private:
  UIText(std::string_view font_name, std::string_view content,
         const CSPill::Math::Vec2D &pos, const CSPill::Math::Vec2D &size,
         int font_size);

  std::string font_name_;
  int font_size_;
  std::string content_;
  CSPill::Math::Vec2D pos_;
  CSPill::Math::Vec2D size_;
  SDL_Surface *text_surface_ = nullptr;
  SDL_Texture *text_texture_ = nullptr;
};

}  // namespace CSPill::UI

#endif  // CSPILLENGINE_ENGINECORE_UITEXT_H_