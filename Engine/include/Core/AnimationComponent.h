#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_

#include <vector>

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

class AnimationComponent : public Component {
 public:
  explicit AnimationComponent(const std::string_view& name);
  AnimationComponent(AnimationComponent&&) = default;
  void Update(GameObject* object, float dt) override;
  void Render(SDL_Renderer* renderer) override;
  std::unordered_map<std::string, std::vector<std::string>> GetAnimations()
      const;
  void AddAnimation(const std::string& name,
                    const std::vector<std::string>& animation);
  void RemoveAnimation(const std::string& name);
  std::string_view GetCurrentAnimation() const;
  void SetCurrentAnimation(const std::string& name);
  int GetCurrentAnimationFrame() const;
  void SetCurrentAnimationFrame(int frame);

 private:
  std::unordered_map<std::string, std::vector<std::string>> animations_;
  std::string current_animation_;
  int current_animation_frame_ = 0;
  SDL_Rect mSrc;
  SDL_Rect mDest;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_