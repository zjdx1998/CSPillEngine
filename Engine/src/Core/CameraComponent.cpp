//
// Created by Jeromy Zhang on 4/16/23.
//

#include "CameraComponent.h"

namespace CSPill::EngineCore {

void CameraComponent::Update(GameObject *object, double dt) {
  Component::Update(object, dt);
  if (this->bind_object_) {

  }
}
void CameraComponent::Bind(GameObject *object, const std::pair<int, int> &offset) {
  this->bind_object_ = object;
  this->offset_ = offset;
}
}  // namespace CSPill::EngineCore
