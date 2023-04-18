//
// Created by Jeromy Zhang on 3/22/23.
//
#include "GameObject.h"

#include "TransformComponent.h"

namespace CSPill::EngineCore {

void GameObject::Update(float dt) {
  for (auto &component : components_) component->Update(this, dt);
}
void GameObject::Render(SDL_Renderer *renderer) {
  for (auto &component : components_) component->Render(renderer);
}
bool GameObject::AddComponent(Component *component) {
  if (component_indices_.find(component->Name()) != component_indices_.end())
    return false;
  components_.push_back(std::unique_ptr<Component>(component));
  component_indices_[components_.back()->Name()] = std::prev(components_.end());
  return true;
}
void GameObject::RemoveComponent(std::string_view component_name) {
  if (component_indices_.find(component_name) != component_indices_.end()) {
    components_.erase(component_indices_[component_name]);
  }
}

std::unique_ptr<GameObject> GameObject::Create() {
  auto *object = new GameObject();
  object->AddComponent(new TransformComponent());
  return std::unique_ptr<GameObject>(object);
}

std::unique_ptr<GameObject> GameObject::Create(
    const CSPill::Math::Vec2D &position, const CSPill::Math::Vec2D &scale) {
  auto *object = new GameObject();
  auto *transform = new TransformComponent();
  transform->position() = position;
  transform->SetScale(scale);
  object->AddComponent(transform);
  return std::unique_ptr<GameObject>(object);
}

Component *GameObject::GetComponent(std::string_view component_name) {
  if (component_indices_.find(component_name) == component_indices_.end())
    return nullptr;
  return component_indices_[component_name]->get();
}

}  // namespace CSPill::EngineCore