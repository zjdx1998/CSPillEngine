//
// Created by Jeromy Zhang on 4/7/23.
//

#include "UI.h"

#include <utility>

namespace CSPill::UI {

UI::UI(std::string title, int width, int height)
    : title_(std::move(title)), width_(width), height_(height) {}
const std::string &UI::GetTitle() const { return title_; }
void UI::SetTitle(const std::string &title) { title_ = title; }
int UI::GetWidth() const { return width_; }
void UI::SetWidth(int width) { width_ = width; }
int UI::GetHeight() const { return height_; }
void UI::SetHeight(int height) { height_ = height; }
bool UI::IsDisplay() const { return display_; }
void UI::SetDisplay(bool display) { display_ = display; }

}  // namespace CSPill::UI