#include "UIText.h"

#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

std::unique_ptr<GameObject> UIText::Create(std::string_view font_name,
                                           std::string_view content,
                                           const CSPill::Math::Vec2D &pos,
                                           const CSPill::Math::Vec2D &size,
                                           int font_size) {
  auto *ui_text = new UIText(font_name, content, pos, size, font_size);
  return std::unique_ptr<GameObject>(ui_text);
}

UIText::UIText(std::string_view font_name, std::string_view content,
               const CSPill::Math::Vec2D &pos, const CSPill::Math::Vec2D &size,
               int font_size)
    : GameObject(),
      font_name_(font_name),
      content_(content),
      pos_(pos),
      size_(size),
      font_size_(font_size),
      text_surface_(nullptr),
      text_texture_(nullptr) {}

const std::string &UIText::GetFontName() const { return font_name_; }

void UIText::SetFontName(const std::string &font_name) {
  font_name_ = font_name;
}

int UIText::GetFontSize() const { return font_size_; }

void UIText::SetFontSize(int font_size) { font_size_ = font_size; }

const CSPill::Math::Vec2D &UIText::GetSize() const { return size_; }

void UIText::SetSize(const CSPill::Math::Vec2D &size) { size_ = size; }

const CSPill::Math::Vec2D &UIText::GetPos() const { return pos_; }

void UIText::SetPos(const CSPill::Math::Vec2D &pos) { pos_ = pos; }

const std::string &UIText::GetContent() const { return content_; }

void UIText::SetContent(const std::string &content) { content_ = content; }

SDL_FRect UIText::GetBoundingBox() {
  return {pos_.x - size_.x / 2, pos_.y - size_.y / 2, size_.x, size_.y};
}

void UIText::Render(SDL_Renderer *renderer) {
  // Load a font
  TTF_Font *font = ResourceManager::GetInstance().LoadFont(font_name_);
  TTF_SetFontSize(font, font_size_);
  SDL_Color color = {255, 255, 255};
  text_surface_ = TTF_RenderText_Solid(font, content_.c_str(), color);
  text_texture_ = SDL_CreateTextureFromSurface(renderer, text_surface_);
  const auto rect = GetBoundingBox();
  SDL_RenderCopyF(renderer, text_texture_, nullptr, &rect);
  SDL_FreeSurface(text_surface_);
}

UIText::~UIText() { SDL_DestroyTexture(text_texture_); }

}  // namespace CSPill::EngineCore