//
// Created by Jeromy Zhang on 3/21/23.
//

#ifndef CSPILLENGINE_ENGINECORE_COMPONENT_H_
#define CSPILLENGINE_ENGINECORE_COMPONENT_H_

#include <string>
#include <memory>
#include <SDL.h>

namespace CSPill::EngineCore {

class GameObject;

class Component{
 public:
  // Virtual Destructor.
  virtual ~Component() = default;
  virtual void Update(GameObject* object) = 0;
  virtual void Render(SDL_Renderer* renderer);
  std::string_view Name();
  virtual void Receive(std::string_view message);

 protected:
  std::string name_;
  explicit Component(std::string_view name="Component");
};

}  // namespace CSPill::EngineCore

#endif //CSPILLENGINE_ENGINECORE_COMPONENT_H_
