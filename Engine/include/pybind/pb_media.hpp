//
// Created by Jeromy Zhang on 4/16/23.
//

#ifndef CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_MEDIA_HPP_
#define CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_MEDIA_HPP_

#include "ResourceManager.h"
#include "SDL_mixer.h"

namespace py = pybind11;

namespace CSPill::Media {

void PB_Media(py::module &m) {
  m.def(
      "PlayMusic",
      [](std::string music_name, int channel = -1, int loops = 0) -> bool {
        if (auto audio = EngineCore::ResourceManager::GetInstance().LoadAudio(
                music_name)) {
          Mix_PlayChannel(channel, audio, loops);
        }
        return false;
      },
      py::arg("music_name"), py::arg("channel") = -1, py::arg("loops") = 0,
      "Play Music by Name, make sure you've loaded the resource in the "
      "ResourceManager.");
  m.def(
      "StopMusic", [](int channel = 1) -> void { Mix_HaltChannel(channel); },
      py::arg("channel") = -1,
      "Stop the Music you've just played, or specify which channel to stop.");
}

}  // namespace CSPill::Media

#endif  // CSPILLENGINE_ENGINE_INCLUDE_PYBIND_PB_MEDIA_HPP_
