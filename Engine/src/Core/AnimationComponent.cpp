#include "AnimationComponent.h"

#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

void AnimationComponent::Update(GameObject* object, float dt) {
  current_animation_frame_ = dt;

  if (current_animation_frame_ > 10) {
    current_animation_frame_ = 0;
  }

  mSrc.x = current_animation_frame_ * 75;
  mSrc.y = 0;
  mSrc.w = 128;
  mSrc.h = 128;
  auto transform_component = (TransformComponent*)(object->GetComponent(
      ::EngineCore::Utils::TRANSFORM_COMPONENT));
  Math::Vec2D pos = transform_component->position();
  mDest.x = pos.x;
  mDest.y = pos.y;
  mDest.w = 128;
  mDest.h = 128;
}
void AnimationComponent::Render(SDL_Renderer* renderer) {
  std::string query =
      current_animation_ + "-cropped-" + std::to_string(current_animation_frame_);
  SDL_Texture* mTexture = ResourceManager::GetInstance().QueryTexture(query);
  SDL_RenderCopy(renderer, mTexture, &mSrc, &mDest);
}
std::unordered_map<std::string, std::vector<std::string>>
AnimationComponent::GetAnimations() const {
  return animations_;
}
void AnimationComponent::AddAnimation(
    const std::string& name, const std::vector<std::string>& animation) {
  animations_[name] = animation;
}
void AnimationComponent::RemoveAnimation(const std::string& name) {
  for (auto it = animations_.begin(); it != animations_.end(); it++) {
    if (it->first.compare(name)) {
      animations_.erase(it);
      break;
    }
  }
}
std::string_view AnimationComponent::GetCurrentAnimation() const {
  return current_animation_;
}
void AnimationComponent::SetCurrentAnimation(const std::string& name) {
  current_animation_ = name;
}
int AnimationComponent::GetCurrentAnimationFrame() const {
  return current_animation_frame_;
}
void AnimationComponent::SetCurrentAnimationFrame(int frame) {
  current_animation_frame_ = frame;
}
AnimationComponent::AnimationComponent()
    : Component(::EngineCore::Utils::ANIMATION_COMPONENT) {}

}  // namespace CSPill::EngineCore