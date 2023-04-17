//
// Created by Jeromy Zhang on 3/21/23.
//

#ifndef CSPILLENGINE_ENGINECORE_COMPONENT_H_
#define CSPILLENGINE_ENGINECORE_COMPONENT_H_

#include <SDL.h>

#include <memory>
#include <string>

namespace CSPill::EngineCore {

class GameObject;

/**
 * \brief Component of GameObject.
 *
 * Interface, some existed inherited implementations are PhysicsComponent,
 * ControllerComponent, etc.
 */
class Component {
 public:
  /**
   * Virtual Destructor.
   */
  virtual ~Component() = default;

  explicit Component(std::string_view name = "Component");
  Component(Component &&) = default;

  /**
   * Update is called every frame, will notify other component through
   * GameObject. This function must be implemented in child class.
   * @param object The GameObject that current component belongs to.
   */
  virtual void Update(GameObject *object, float dt) = 0;

  /**
   * Render is called every frame.
   * @param renderer SDL_Renderer renderer.
   */
  virtual void Render(SDL_Renderer *renderer);

  /**
   * Name of the component.
   * @return a string_view object, represents the name of component.
   */
  std::string_view Name();

  /**
   * Describe the behavior when it receives message from other components.
   * @param message content of the message.
   */
  virtual void Receive(std::string_view message);

 protected:
  std::string name_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINECORE_COMPONENT_H_
