// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows,
// inputs, OpenGL/Vulkan/Metal graphics context creation, etc.) If you are new
// to Dear ImGui, read documentation from the docs/ folder + read the top of
// imgui.cpp. Read online: https://github.com/ocornut/imgui/tree/master/docs

// Important to understand: SDL_Renderer is an _optional_ component of SDL. We
// do not recommend you use SDL_Renderer because it provides a rather limited
// API to the end-user. We provide this backend for the sake of completeness.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and
// SDL+OpenGL on Linux/OSX.
#define SDL_MAIN_HANDLED
#include <EditorScene.h>
#include <ResourceManager.h>
#include <SDL.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Engine.h"
#include "ImGuiFileDialog.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_internal.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

namespace {

using CSPill::Editor::ResourceManagerUI;
using CSPill::Editor::SceneUI;
using CSPill::Editor::TileSetEditorUI;
using CSPill::EngineCore::Engine;
using json = nlohmann::json;
using CSPill::EngineCore::Scene;

bool create_new_window = false;
bool save_scene = false;
bool save_scene_as = false;

constexpr float FILE_BROWSER_WIDTH = 400;
constexpr float FILE_BROWSER_HEIGHT = 300;

bool CreateFile(std::string_view path, std::string_view content) {
  std::ofstream file(path.data());
  if (!file) return false;
  file << content;
  file.close();
  return true;
}

bool CreateFolder(std::string_view path) {
  return std::filesystem::create_directories(path.data());
}

void MenuBar(bool &done) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Project")) {
        create_new_window = true;
      }
      if (ImGui::MenuItem("Save Scene")) {
        save_scene = true;
      }
      if (ImGui::MenuItem("Save Scene As...")) {
        save_scene_as = true;
      }
      ImGui::Text("------");
      if (ImGui::MenuItem("Exit", "Cmd+Q")) {
        done = true;
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  // create a new project
  if (create_new_window) {
    // create a dialog
    ImGui::OpenPopup("Create Project");

    if (ImGui::BeginPopupModal("Create Project", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      // input filename
      static char name[256] = "untitled";
      ImGui::InputText("Name", name, IM_ARRAYSIZE(name));

      auto default_path =
          std::getenv("HOME") + std::string("/CSPillEngineProjects/");
      ImGui::InputText("Save Location", default_path.data(),
                       default_path.size());

      if (ImGui::Button("Create")) {
        std::string folder_path = std::string(default_path) + name;
        folder_path.erase(
            std::remove(folder_path.begin(), folder_path.end(), '\n'),
            folder_path.end());
        std::cout << folder_path << std::endl;
        if (!std::filesystem::exists(folder_path)) {
          std::cout << CreateFolder(folder_path) << std::endl;
          folder_path += "/";

          CreateFolder((folder_path + "src").c_str());
          CreateFile(folder_path + "src/" + "app.py", "");
          CreateFolder((folder_path + "resources").c_str());
          CreateFolder((folder_path + "scenes").c_str());

          CreateFile(folder_path + "scenes/" + "default.scene", "");
          create_new_window = false;
        } else {
          std::cerr << "Directory already exists";
        }
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        create_new_window = false;
      }

      ImGui::EndPopup();
    }
  }

  if (save_scene) {
    Scene *scene_to_save =
        CSPill::EngineCore::ResourceManager::GetInstance().ActiveScene();

    json json_object = *scene_to_save;

    std::string path_to_scene =
        CSPill::EngineCore::ResourceManager::GetInstance()
            .GetActiveScenePath();  // read path from scene_to_save

    std::cout << path_to_scene << std::endl;

    std::ofstream file(path_to_scene);

    file << json_object;
    file.close();
    save_scene = false;
  }

  if (save_scene_as) {
    ImGui::OpenPopup("Save Scene As");
    if (ImGui::BeginPopupModal("Save Scene As", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      Scene *scene_to_save =
          CSPill::EngineCore::ResourceManager::GetInstance().ActiveScene();

      json json_object = *scene_to_save;
      static std::string file_path;

      ImGui::InputText("File Path", file_path.data(), file_path.capacity());
      ImGui::SameLine();
      if (ImGui::Button("Browse")) {
        // open Dialog Simple
        ImGui::SetNextWindowSize(
            ImVec2(FILE_BROWSER_WIDTH, FILE_BROWSER_HEIGHT));
        ImGuiFileDialog::Instance()->OpenDialog("FileBrowser", "Choose Folder",
                                                nullptr, ".");
      }

      // Display file browser
      if (ImGuiFileDialog::Instance()->Display("FileBrowser")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
          file_path = ImGuiFileDialog::Instance()->GetFilePathName();
        }
        ImGuiFileDialog::Instance()->Close();
      }

      // Confirm button
      if (ImGui::Button("Confirm")) {
        std::cout << file_path << std::endl;

        std::ofstream file(file_path + "/default.scene");

        file << json_object;
        file.close();
        ImGui::CloseCurrentPopup();
        save_scene_as = false;
      }

      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        save_scene_as = false;
      }

      ImGui::EndPopup();
    }
  }
}

}  // namespace

// Main code
int main(int argc, char **argv) {
  auto engine = Engine::Create("CSPill Engine Editor", 1280, 720);

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  if (argc > 1) {
    CSPill::EngineCore::ResourceManager::GetInstance().LoadResources(argv[1]);
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Enable docking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(engine->GetWindow(), engine->GetRenderer());
  ImGui_ImplSDLRenderer_Init(engine->GetRenderer());

  // Our state
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Init ResourceUI
  TileSetEditorUI layers_widget("TileSets", 300, 800);

  // Init SceneUI
  // TODO: load json
  // scene_widget.LoadScene();
  SceneUI scene_widget("Scene", 800, 600);
  scene_widget.SetPreview(true);

  // Init ResourceManagerUI
  ResourceManagerUI resource_manager_widget("Resource Manager", 600, 300);

  bool dockspace_open = true;
  ImGuiWindowFlags docking_window_flags =
      ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoMove;

  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) done = true;
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(engine->GetWindow()))
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainWindow", &dockspace_open, docking_window_flags);
    ImGui::PopStyleVar();

    ImGuiID dockspace_id = ImGui::GetID("DefaultDockSpace");
    const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
    ImGui::DockSpace(
        dockspace_id, ImVec2(0.0f, 0.0f),
        ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton);

    // Render Resources widget
    layers_widget.Render(engine->GetRenderer());

    // Render Scene widget
    scene_widget.Render(engine->GetRenderer());

    // Render Resource Manager widget
    resource_manager_widget.Render(engine->GetRenderer());

    static bool docking_init = true;
    // Set up docking
    if (docking_init) {
      docking_init = false;
      // Clear previous docking layout
      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
      ImGui::DockBuilderSetNodeSize(dockspace_id, dockspace_size);

      ImGuiID dock_main_id = dockspace_id;
      ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(
          dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
      // Set TileSets panel
      ImGui::DockBuilderDockWindow("TileSets", dock_left_id);
      ImGuiID dock_left_bottom_id = ImGui::DockBuilderSplitNode(
          dock_left_id, ImGuiDir_Down, 0.5f, nullptr, &dock_left_id);
      // Set Resource Manager panel
      ImGui::DockBuilderDockWindow("Resource Manager", dock_left_bottom_id);
      // Set Scene panel
      ImGui::DockBuilderDockWindow("Scene", dock_main_id);

      // Finalize the dock layout
      ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();

    MenuBar(done);

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(engine->GetRenderer(), io.DisplayFramebufferScale.x,
                       io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(engine->GetRenderer(), (Uint8)(clear_color.x * 255),
                           (Uint8)(clear_color.y * 255),
                           (Uint8)(clear_color.z * 255),
                           (Uint8)(clear_color.w * 255));
    SDL_RenderClear(engine->GetRenderer());
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(engine->GetRenderer());
  }

  // Cleanup
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  CSPill::EngineCore::ResourceManager::GetInstance().ReleaseAll();

  // SDL_DestroyRenderer(engine->GetRenderer());
  // SDL_DestroyWindow(window);
  // SDL_Quit();

  return 0;
}
