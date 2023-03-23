//
// Created by Jeromy Zhang on 3/22/23.
//

#include <iostream>
#include "Component.h"

namespace CSPill::EngineCore {

Component::Component(std::string_view name) : name_(name) {

}
std::string_view Component::Name() {
  return name_;
}
void Component::Receive(std::string_view message) {
  std::cout << "Message: " << message << " received!\n";
}
void Component::Render(SDL_Renderer *renderer) {

}

}  // namespace CSPill::EngineCore