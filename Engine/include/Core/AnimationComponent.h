#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_

#include <vector>

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

/**
 * \brief Animation Component
 *
 * Animation Component takes care of all the animation made.
 */
class AnimationComponent : public Component {
 public:
  /**
   * Constructor of AnimationComponent.
   */
  explicit AnimationComponent();

  AnimationComponent(AnimationComponent &&) = default;

  /**
   * Update the target GameObject by dt time.
   * @param object GameObject that is needed to be updated
   * @param dt float, time elapsed
   */
  void Update(GameObject *object, float dt) override;

  /**
   * Render the animation.
   * @param renderer SDL_Renderer that is going to be used
   */
  void Render(SDL_Renderer *renderer) override;

  /**
   * Get the animation map.
   * @return unordered_map contains all animations
   */
  [[nodiscard]] const std::unordered_map<std::string, std::vector<std::string>>
      &GetAnimations() const;

  /**
   * Add an animation by Tileset and tile number.
   * @param name a string of name of Tileset
   * @param animations a vector of string of the tile number
   * @return true if successfully added, false otherwise
   */
  bool AddAnimations(const std::string &name,
                     const std::vector<std::string> &animations);

  /**
   * Add an animation by name.
   * @param name a string of name of image
   * @param animation a string of name of image
   * @return true if successfully added, false otherwise
   */
  bool AddAnimation(const std::string &name, const std::string &animation);

  /**
   * Remove an animation by name.
   * @param name a string of name of the animation
   */
  void RemoveAnimation(const std::string &name);

  /**
   * Get the name of the current animation playing.
   * @return a string of name of the animation
   */
  [[nodiscard]] std::string_view GetCurrentAnimation() const;

  /**
   * Set current animation to name.
   * @param name a string of name of the animation
   */
  void SetCurrentAnimation(const std::string &name);

  /**
   * Get current frame.
   * @return float, current frame
   */
  [[nodiscard]] float GetFrame() const;

  /**
   * Set the frame to target float.
   * @param frame float, target frame
   */
  void SetFrame(float frame);

  /**
   * Get current speed.
   * @return float, the playing speed
   */
  [[nodiscard]] float GetSpeed() const;

  /**
   * Set playing speed.
   * @param speed float, the playing speed
   */
  void SetSpeed(float speed);

 private:
  std::unordered_map<std::string, std::vector<std::string>> animations_;
  std::string current_animation_;
  float frame_ = 0;
  float speed_ = 30;  // 30 frames per second;
  SDL_FRect dst_rect_;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_ANIMATION_COMPONENT_H_