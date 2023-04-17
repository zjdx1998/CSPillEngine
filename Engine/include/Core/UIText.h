#ifndef CSPILLENGINE_ENGINECORE_UITEXT_H_
#define CSPILLENGINE_ENGINECORE_UITEXT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {
class UIText : public GameObject {
 public:
  static std::unique_ptr<GameObject> Create(std::string_view font_name_,
                                            std::string_view content,
                                            const CSPill::Math::Vec2D &pos,
                                            const CSPill::Math::Vec2D &size,
                                            int font_size);
  UIText(UIText &&) = default;
  ~UIText() override;
  [[nodiscard]] const std::string &GetFontName() const;
  void SetFontName(const std::string &font_name);
  [[nodiscard]] int GetFontSize() const;
  void SetFontSize(int font_size);
  [[nodiscard]] const CSPill::Math::Vec2D &GetSize() const;
  void SetSize(const CSPill::Math::Vec2D &size);
  [[nodiscard]] const CSPill::Math::Vec2D &GetPos() const;
  void SetPos(const CSPill::Math::Vec2D &pos);
  [[nodiscard]] const std::string &GetContent() const;
  void SetContent(const std::string &content);
  void Render(SDL_Renderer *renderer) override;
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

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINECORE_UITEXT_H_