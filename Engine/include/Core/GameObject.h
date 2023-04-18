//
// Created by Jeromy Zhang on 3/21/23.
//

#ifndef CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_
#define CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_

#include <list>
#include <unordered_map>

#include "Component.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {
/**
 * \brief Base class for all entities in CSPill Scenes.
 *
 * GameObject is the minimal unit to represent an object, it can be composed of
 * multiple components.
 */
class GameObject {
 public:
  /**
   * Factory function to create an GameObject.
   *
   * @return a unique pointer to instantiated game object.
   */
  static std::unique_ptr<GameObject> Create();

  /**
   * Factory function to create an GameObject with position and scale.
   *
   * @param position position of game object
   * @param scale scale of game object
   * @return a unique pointer to instantiated game object.
   */
  static std::unique_ptr<GameObject> Create(const CSPill::Math::Vec2D &position,
                                            const CSPill::Math::Vec2D &scale);

  /**
   * Virtual Destructor
   */
  virtual ~GameObject() = default;

  /**
   * Default move constructor
   */
  GameObject(GameObject &&) = default;

  /**
   * Update is called every frame, if the object is enabled.
   * @param dt float, time elapsed
   */
  void Update(float dt);

  /**
   * Render is called every frame, if the object is enabled.
   * @param renderer SDL_Renderer renderer.
   */
  virtual void Render(SDL_Renderer *renderer);
  /**
   * Add a component to current GameObject.
   * @param component new component that needs to be added.
   * @return False if it's duplicated.
   */
  bool AddComponent(Component *component);

  /**
   * Remove a component based on the component name.
   * @param component_name name of the component.
   */
  void RemoveComponent(std::string_view component_name);

  /**
   * Get a raw pointer to component based on name.
   * @param component_name name of the component
   * @return Raw pointer of the targeted component.
   */
  Component *GetComponent(std::string_view component_name);

  /**
   * Is the current object live or not?
   * @return true if it lives.
   */
  [[nodiscard]] bool IsLive() const;
  /**
   * Set the live state of the object.
   * @param live true or false.
   */
  void SetLive(bool live);

 protected:
  GameObject() = default;

 private:
  std::list<std::unique_ptr<Component>> components_;
  std::unordered_map<std::string_view,
                     std::list<std::unique_ptr<Component>>::iterator>
      component_indices_;
  bool live_ = true;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINECORE_GAMEOBJECT_H_
