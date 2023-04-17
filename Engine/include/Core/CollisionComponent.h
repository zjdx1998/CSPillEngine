//
// Created by Jeromy Zhang on 4/17/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_

#include <unordered_set>
#include <functional>

#include "Component.h"

namespace CSPill::EngineCore {

class CollisionComponent : public Component {
 public:
  explicit CollisionComponent(const SDL_FRect &bounding_box);
  // NO PYBIND
  void Update(GameObject *object, float dt) override;
  [[nodiscard]] const SDL_FRect &GetBoundingBox() const;
  void SetBoundingBox(const SDL_FRect &bounding_box);
  [[nodiscard]] const std::function<void(GameObject *)> &GetCallback() const;
  void SetCallback(const std::function<void(GameObject *)> &callback);
  void Register(GameObject *object);
  void UnRegister(GameObject *object);
  static bool Collide(const SDL_FRect &A, const SDL_FRect &B);

 private:
  SDL_FRect bounding_box_;
  std::unordered_set<GameObject *> registers_;
  std::function<void(GameObject *)> callback_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
