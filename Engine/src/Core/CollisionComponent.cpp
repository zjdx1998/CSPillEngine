//
// Created by Jeromy Zhang on 4/17/23.
//

#include "CollisionComponent.h"

namespace CSPill::EngineCore {

void CollisionComponent::Update(GameObject *object, float dt) {}
const SDL_FRect &CollisionComponent::GetBoundingBox() const {
  return bounding_box_;
}
void CollisionComponent::SetBoundingBox(const SDL_FRect &bounding_box) {
  bounding_box_ = bounding_box;
}
const std::function<void(GameObject *)> &CollisionComponent::GetCallback()
    const {
  return callback_;
}
void CollisionComponent::SetCallback(
    const std::function<void(GameObject *)> &callback) {
  callback_ = callback;
}

}  // namespace CSPill::EngineCore
