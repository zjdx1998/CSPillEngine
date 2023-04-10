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
#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif
#include <EditorScene.h>
#include <ResourceManager.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Scene.h>

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_internal.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

using CSPill::Editor::ResourceManagerUI;
using CSPill::Editor::SceneUI;
using CSPill::Editor::TileSetEditorUI;

// Main code
int main(int, char **) {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
  //   std::cout << "Error SDL2_image Initialization";
  //   return 2;
  // }

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Create window with SDL_Renderer graphics context
  auto window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window *window =
      SDL_CreateWindow("CSPill Engine Editor", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Error creating SDL_Renderer!");
    return 0;
  }
  CSPill::EngineCore::ResourceManager::GetInstance().SetRenderer(renderer);
  // SDL_RendererInfo info;
  // SDL_GetRendererInfo(renderer, &info);
  // SDL_Log("Current SDL_Renderer: %s", info.name);

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
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Init ResourceUI
  TileSetEditorUI layers_widget("TileSets", 300, 800);

  // Init SceneUI
  // TODO: load json
  // scene_widget.LoadScene();
  SceneUI scene_widget("Scene", 800, 600);

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
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) done = true;
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
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
    layers_widget.Render(renderer);

    // Render Scene widget
    scene_widget.Render(renderer);

    // Render Resource Manager widget
    resource_manager_widget.Render(renderer);

    static bool docking_init = true;
    // Set up docking
    if (docking_init) {
      std::cout << "init dockspace" << std::endl;
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

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x,
                       io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(
        renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255),
        (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  CSPill::EngineCore::ResourceManager::GetInstance().ReleaseAll();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
