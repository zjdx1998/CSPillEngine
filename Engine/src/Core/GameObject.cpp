//
// Created by Jeromy Zhang on 3/22/23.
//
#include "GameObject.h"

namespace CSPill::EngineCore {

void GameObject::Update() {
  for (auto &component : components_) component->Update(this);
}
void GameObject::Render(SDL_Renderer *renderer) {
  for (auto &component : components_) component->Render(renderer);
}
bool GameObject::AddComponent(Component &&component) {
  if (component_indices_.find(component.Name()) != component_indices_.end())
    return false;
  components_.emplace_back(std::make_unique<Component>(std::move(component)));
  component_indices_[components_.back()->Name()] = std::prev(components_.end());
  return true;
}
void GameObject::RemoveComponent(std::string_view component_name) {
  if (component_indices_.find(component_name) != component_indices_.end()) {
    components_.erase(component_indices_[component_name]);
  }
}

std::unique_ptr<GameObject> GameObject::Create() {
  return std::unique_ptr<GameObject>(new GameObject());
}
Component *GameObject::GetComponent(std::string_view component_name) {
  if (component_indices_.find(component_name) == component_indices_.end())
    return nullptr;
  return component_indices_[component_name]->get();
}

}  // namespace CSPill::EngineCore