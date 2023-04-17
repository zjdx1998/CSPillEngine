//
// Created by Jeromy Zhang on 4/17/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_

#include "Component.h"

namespace CSPill::EngineCore {

class CollisionComponent : public Component {
 public:
  void Update(GameObject *object, float dt) override;
  [[nodiscard]] const SDL_FRect &GetBoundingBox() const;
  void SetBoundingBox(const SDL_FRect &bounding_box);
  [[nodiscard]] const std::function<void(GameObject *)> &GetCallback() const;
  void SetCallback(const std::function<void(GameObject *)> &callback);

 private:
  SDL_FRect bounding_box_;
  std::function<void(GameObject *)> callback_;
};

}  // namespace CSPill::EngineCore

#endif //CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
