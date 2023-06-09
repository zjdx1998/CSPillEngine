#include "AnimationComponent.h"

#include "CameraComponent.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Utils.h"

namespace CSPill::EngineCore {

void AnimationComponent::Update(GameObject *object, float dt) {
  frame_ += dt / 1200 * speed_;
  if (auto transform = dynamic_cast<TransformComponent *>(
          object->GetComponent(::EngineCore::Utils::TRANSFORM_COMPONENT))) {
    float x = transform->position().x, y = transform->position().y;
    if (auto camera = ResourceManager::GetInstance().GetActiveCamera()) {
      if (auto camera_component = dynamic_cast<CameraComponent *>(
              camera->GetComponent(::EngineCore::Utils::CAMERA_COMPONENT))) {
        x -= camera_component->GetViewport().x;
        y -= camera_component->GetViewport().y;
      }
    }
    dst_rect_ = {x, y, transform->GetScale().x, transform->GetScale().y};
  }
}

void AnimationComponent::Render(SDL_Renderer *renderer) {
  if (animations_.find(current_animation_) == animations_.end()) return;
  int frame = static_cast<int>(round(frame_)) %
              (animations_[current_animation_].size());
  if (frame_ > animations_[current_animation_].size()) {
    frame_ -= animations_[current_animation_].size();
  }
  if (auto sprite = ResourceManager::GetInstance().QueryTexture(
          animations_[current_animation_].at(frame))) {
    int w, h;
    SDL_QueryTexture(sprite, nullptr, nullptr, &w, &h);
    dst_rect_.w *= static_cast<float>(w);
    dst_rect_.h *= static_cast<float>(h);
    SDL_RenderCopyF(renderer, sprite, nullptr, &dst_rect_);
  }
}

const std::unordered_map<std::string, std::vector<std::string>>
    &AnimationComponent::GetAnimations() const {
  return animations_;
}

bool AnimationComponent::AddAnimation(const std::string &name,
                                      const std::string &animation) {
  if (ResourceManager::GetInstance().QueryTexture(animation)) {
    if (animations_.find(name) == animations_.end()) {
      animations_[name] = {};
    }
    animations_[name].emplace_back(animation);
    return true;
  }
  return false;
}

bool AnimationComponent::AddAnimations(
    const std::string &name, const std::vector<std::string> &animations) {
  return std::all_of(
      animations.begin(), animations.end(),
      [&](const auto &animation) { return AddAnimation(name, animation); });
}
void AnimationComponent::RemoveAnimation(const std::string &name) {
  for (auto it = animations_.begin(); it != animations_.end(); it++) {
    if (it->first == name) {
      animations_.erase(it);
      break;
    }
  }
}
std::string_view AnimationComponent::GetCurrentAnimation() const {
  return current_animation_;
}
void AnimationComponent::SetCurrentAnimation(const std::string &name) {
  current_animation_ = name;
}
float AnimationComponent::GetFrame() const { return frame_; }
void AnimationComponent::SetFrame(float frame) { frame_ = frame; }
AnimationComponent::AnimationComponent()
    : Component(::EngineCore::Utils::ANIMATION_COMPONENT),
      dst_rect_({0, 0, 0, 0}) {}
float AnimationComponent::GetSpeed() const { return speed_; }
void AnimationComponent::SetSpeed(float speed) {
  AnimationComponent::speed_ = speed;
}

}  // namespace CSPill::EngineCore