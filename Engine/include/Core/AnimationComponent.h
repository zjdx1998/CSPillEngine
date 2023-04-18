#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_

#include <vector>

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

class AnimationComponent : public Component {
 public:
  explicit AnimationComponent();
  AnimationComponent(AnimationComponent &&) = default;
  void Update(GameObject *object, float dt) override;
  void Render(SDL_Renderer *renderer) override;
  [[nodiscard]] const std::unordered_map<std::string, std::vector<std::string>> &GetAnimations()
  const;
  bool AddAnimations(const std::string &name,
                     const std::vector<std::string> &animations);
  bool AddAnimation(const std::string &name,
                    const std::string &animation);
  void RemoveAnimation(const std::string &name);
  [[nodiscard]] std::string_view GetCurrentAnimation() const;
  void SetCurrentAnimation(const std::string &name);
  [[nodiscard]] float GetFrame() const;
  void SetFrame(float frame);
  [[nodiscard]] float GetSpeed() const;
  void SetSpeed(float speed);

 private:
  std::unordered_map<std::string, std::vector<std::string>> animations_;
  std::string current_animation_;
  float frame_ = 0;
  float speed_ = 30; // 30 frames per second;
  SDL_FRect dst_rect_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_