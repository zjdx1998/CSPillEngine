//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_

#include "GameObject.h"
#include "TinyMath.hpp"

namespace CSPill::EngineCore {

  /**
   * \brief Transform Component
   *
   * Transform Component takes care of all objects' displacement.
   */
  class TransformComponent : public Component {
  public:
    /**
     * Constructor of TransformComponent.
     */
    explicit TransformComponent();

    TransformComponent(TransformComponent &&) = default;

    ~TransformComponent() override = default;

    /**
     * Update the target GameObject by dt time.
     * @param object GameObject that is needed to be updated
     * @param dt float, time elapsed
     */
    void Update(GameObject *object, float dt) override;

    /**
     * Get the position.
     * @return Vec2D, which represents x and y location
     */
    Math::Vec2D &position();

    /**
     * Get the velocity.
     * @return Vec2D, which represents x and y velocity
     */
    Math::Vec2D &velocity();

    /**
     * Get the scala.
     * @return Vec2D, which represents x and y coordinate
     */
    const Math::Vec2D &GetScale() const;

    /**
     * Set the scala.
     * @param scale Vec2D, which represents x and y coordinate
     */
    void SetScale(const Math::Vec2D &scale);

  private:
    Math::Vec2D position_;
    Math::Vec2D velocity_;
    Math::Vec2D scale_;
  };

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_TRANSFORMCOMPONENT_H_
