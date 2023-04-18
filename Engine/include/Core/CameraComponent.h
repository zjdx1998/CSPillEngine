//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

/**
 * \brief Camera Component
 *
 * Camera component controls the area that user can see.
 */
class CameraComponent : public Component {
 public:
  /**
   * Constructor of CameraComponent.
   */
  explicit CameraComponent();

  /**
   * Destructor of CameraComponent.
   */
  ~CameraComponent() override;

  /**
   * Update the target GameObject by dt time.
   * @param object GameObject that is needed to be updated
   * @param dt float, time elapsed
   */
  void Update(GameObject *object, float dt) override;

  /**
   * Bind the GameObject to the camera.
   * @param object GameObject that is going to be binded
   * @param view_range Vec2D, the width and height that camera shows
   */
  void Bind(GameObject *object, const Math::Vec2D &view_range = {0, 0});

  /**
   * Get the view port of the camera.
   * @return SDL_FRect, the view port of the camera
   */
  const SDL_FRect &GetViewport();

  /**
   * Set camera view to certain width and height.
   * @param w float, the width of view
   * @param h float, the height of view
   */
  void SetViewport(float w, float h);

  /**
   * Get the range that the camera shows.
   * @return Vec2D, represents the area that the camera shows
   */
  [[nodiscard]] const Math::Vec2D &GetViewRange() const;

  /**
   * Get the range that the camera shows.
   * @param view_range Vec2D, represents the area that the camera shows
   */
  void SetViewRange(const Math::Vec2D &view_range);

  /**
   * Get the distance that the camera travels.
   * @return float, the distance that the camera travels
   */
  float GetTransit() const;

  /**
   * Set the distance that the camera travels.
   * @param transit float, the distance that the camera travels
   */
  void SetTransit(float transit);

 private:
  SDL_FRect viewport_{};
  Math::Vec2D offset_;
  Math::Vec2D view_range_;
  GameObject *bind_object_{};
  float transit_ = 0.5;
};

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_CAMERA_COMPONENT_H_
