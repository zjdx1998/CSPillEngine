//
// Created by Jeromy Zhang on 3/21/23.
//

#ifndef CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_
#define CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_

#include <list>
#include <unordered_map>
#include "Component.h"

namespace CSPill::EngineCore {

class GameObject{
 public:
  static std::unique_ptr<GameObject> Create();
  ~GameObject() = default;

  void Update();
  void Render(SDL_Renderer* renderer);
  bool AddComponent(std::unique_ptr<Component> component);
  void RemoveComponent(std::string_view component_name);
  Component* GetComponent(std::string_view component_name);

 private:
  GameObject() = default;
  std::list<std::unique_ptr<Component>> components_;
  std::unordered_map<std::string_view, std::list<std::unique_ptr<Component>>::iterator> component_indices_;
};

}  // namespace CSPill::EngineCore

#endif //CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_
