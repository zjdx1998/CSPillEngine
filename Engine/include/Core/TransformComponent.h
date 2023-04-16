//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

class TransformComponent : public Component {
 public:
  void Update(GameObject *object, double dt) override;
 private:
  Math::Vec2D position_;
  Math::Vec2D velocity_;
};

}  // CSPill::EngineCore

#endif //CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_
