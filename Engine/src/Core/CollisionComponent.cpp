//
// Created by Jeromy Zhang on 4/17/23.
//

#include "CollisionComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

bool CollisionComponent::Collide(const SDL_FRect &A, const SDL_FRect &B) {
  bool horizontal_overlap = (A.x < B.x + B.w) && (A.x + A.w > B.x);
  bool vertical_overlap = (A.y < B.y + B.h) && (A.y + A.h > B.y);
  return horizontal_overlap && vertical_overlap;
}

void CollisionComponent::Update(GameObject *object, float dt) {
  if (auto transform = dynamic_cast<TransformComponent *>(
          object->GetComponent(::EngineCore::Utils::TRANSFORM_COMPONENT))) {
    bounding_box_.x = transform->position().x;
    bounding_box_.y = transform->position().y;
  }
  for (auto obj : registers_) {
    if (auto temp_other =
            obj->GetComponent(::EngineCore::Utils::COLLISION_COMPONENT)) {
      auto other = dynamic_cast<CollisionComponent *>(temp_other);
      if (Collide(this->bounding_box_, other->bounding_box_)) {
        if (callback_) {
          callback_(object, obj);
        }
        if (other->callback_) {
          other->callback_(obj, object);
        }
      }
    }
  }
}

const SDL_FRect &CollisionComponent::GetBoundingBox() const {
  return bounding_box_;
}
void CollisionComponent::SetBoundingBox(const SDL_FRect &bounding_box) {
  bounding_box_ = bounding_box;
}
const std::function<void(GameObject *, GameObject *)>
    &CollisionComponent::GetCallback() const {
  return callback_;
}
void CollisionComponent::SetCallback(
    const std::function<void(GameObject *, GameObject *)> &callback) {
  callback_ = callback;
}
void CollisionComponent::Register(GameObject *object) {
  if (object) {
    registers_.insert(object);
  }
}
void CollisionComponent::UnRegister(GameObject *object) {
  if (object && registers_.find(object) != registers_.end()) {
    registers_.erase(object);
  }
}
CollisionComponent::CollisionComponent(const SDL_FRect &bounding_box)
    : bounding_box_(bounding_box),
      Component(::EngineCore::Utils::COLLISION_COMPONENT) {}

}  // namespace CSPill::EngineCore
