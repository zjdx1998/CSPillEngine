//
// Created by Jeromy Zhang on 4/17/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
#define CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_

#include <functional>
#include <unordered_set>

#include "Component.h"

namespace CSPill::EngineCore {

  /**
   * \brief Physics Collision Component
   *
   * Collision Component handles all the physics of the game.
   */
  class CollisionComponent : public Component {
  public:
    /**
     * Constructor of CollisionComponent.
     * @param bounding_box SDL_FRect, hit box of an object
     */
    explicit CollisionComponent(const SDL_FRect &bounding_box);

    /**
     * Update the target GameObject by dt time.
     * @param object GameObject that is needed to be updated
     * @param dt float, time elapsed
     */
    void Update(GameObject *object, float dt) override;

    /**
     * Get the hit box.
     * @return SDL_FRect, hit box of an object
     */
    [[nodiscard]] const SDL_FRect &GetBoundingBox() const;

    /**
     * Set hit box.
     * @param bounding_box SDL_FRect, hit box of an object
     */
    void SetBoundingBox(const SDL_FRect &bounding_box);

    /**
     * Get callback function.
     * @return callback function
     */
    [[nodiscard]] const std::function<void(GameObject *)> &GetCallback() const;

    /**
     * Set callback function.
     * @param callback callback function
     */
    void SetCallback(const std::function<void(GameObject *)> &callback);

    /**
     * Register a GameObject to Collision Component
     * @param object GameObject that is going to be registered
     */
    void Register(GameObject *object);

    /**
     * Unregister a GameObject to Collision Component
     * @param object GameObject that is going to be unregister
     */
    void UnRegister(GameObject *object);

    /**
     * Check if two hit box collide
     * @param A SDL_FRect, hit box
     * @param B SDL_FRect, hit box
     * @return true if they collide, false otherwise
     */
    static bool Collide(const SDL_FRect &A, const SDL_FRect &B);

  private:
    SDL_FRect bounding_box_;
    std::unordered_set<GameObject *> registers_;
    std::function<void(GameObject *)> callback_;
  };

}  // namespace CSPill::EngineCore

#endif  // CSPILLENGINE_ENGINE_INCLUDE_CORE_COLLISIONCOMPONENT_H_
