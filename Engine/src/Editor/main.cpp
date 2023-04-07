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
#include <SDL.h>

#include <cstdio>
#include <iostream>
#include <Component.h>
#include <filesystem>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

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
      (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window *window =
      SDL_CreateWindow("CSPill Engine Editor", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    SDL_Log("Error creating SDL_Renderer!");
    return 0;
  }
  // SDL_RendererInfo info;
  // SDL_GetRendererInfo(renderer, &info);
  // SDL_Log("Current SDL_Renderer: %s", info.name);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
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

  // selected resource index
  int selected_resource_index = -1;

  const int SCENE_NUM_ROWS = 10;
  const int SCENE_NUM_COLS = 10;
  const int SCENE_BLOCK_SIZE = 50;
  SDL_Texture *scene_textures[SCENE_NUM_ROWS * SCENE_NUM_COLS] = {nullptr};;

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

    ImGui::DockSpaceOverViewport();
    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!"
      // and append into it.

      ImGui::Text("This is some useful text.");  // Display some text (you can
      // use a format strings too)
      ImGui::Checkbox(
          "Demo Window",
          &show_demo_window);  // Edit bools storing our window open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      ImGui::SliderFloat(
          "float", &f, 0.0f,
          1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3(
          "clear color",
          (float *) &clear_color);  // Edit 3 floats representing a color

      if (ImGui::Button(
          "Button"))  // Buttons return true when clicked (most widgets
        // return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      ImGui::Begin(
          "Another Window",
          &show_another_window);  // Pass a pointer to our bool variable (the
      // window will have a closing button that will
      // clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")) show_another_window = false;
      ImGui::End();
    }

    // Show resources widget
    ImGui::Begin("Resources");

    // Specify the folder containing the image files
    // std::string folder_path = "../demo/resources/images";
    std::string folder_path = "../demo/resources/images";

    // Get a list of all the image files in the folder
    std::vector<std::string> file_paths;
    for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
      if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
        file_paths.push_back(entry.path().string());
      }
    }

    // Calculate the number of rows and columns needed to display the images
    const int RESOURCES_NUM_COLS = 3;
    int num_rows = static_cast<int>(std::ceil(static_cast<double>(file_paths.size()) / RESOURCES_NUM_COLS));

    SDL_Texture *resource_textures[file_paths.size()];

    // Load and display the images in a grid
    for (int row = 0; row < num_rows; row++) {
      for (int col = 0; col < RESOURCES_NUM_COLS; col++) {
        int file_index = row * RESOURCES_NUM_COLS + col;
        if (file_index < file_paths.size()) {
          // Render scene component to SDL texture
          std::string image_filename = "../demo/resources/sprites/adventurer-v1.5-Sheet.bmp";
          SDL_Surface *mSurface = SDL_LoadBMP(image_filename.c_str());
          SDL_Texture *mTexture = SDL_CreateTextureFromSurface(renderer, mSurface);
          resource_textures[row * RESOURCES_NUM_COLS + col] = mTexture;

          // Display the image in the ImGUI window
          ImGui::PushID(file_index);
          ImGui::Image(mTexture, ImVec2(100, 100));
          ImGui::PopID();

          if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            selected_resource_index = row * RESOURCES_NUM_COLS + col;
            std::cout << "clicked resource: " << row * RESOURCES_NUM_COLS + col << std::endl;
          }
        } else {
          // If there are no more image files, display an empty space
          ImGui::Dummy(ImVec2(100, 100));
        }

        // Add a same-line separator to align images horizontally
        if (col < RESOURCES_NUM_COLS - 1) {
          ImGui::SameLine();
        }
      }
    }

    ImGui::End();

    // test SDL rendering to widget
    ImGui::Begin("Scene");
    // Render scene component to SDL texture
    std::string image_filename = "../demo/resources/sprites/adventurer-v1.5-Sheet.bmp";
    SDL_Surface *mSurface = SDL_LoadBMP(image_filename.c_str());
    SDL_Texture *mTexture = SDL_CreateTextureFromSurface(renderer, mSurface);

    // Calculate the width and height of each frame in the BMP file
    int frame_width = mSurface->w / 7;
    int frame_height = mSurface->h / 16;

    // Test rendering blocks individually
    for (int row = 0; row < SCENE_NUM_ROWS; row++) {
      for (int col = 0; col < SCENE_NUM_COLS; col++) {
        int block_index = row * SCENE_NUM_COLS + col;
        if (scene_textures[block_index] != nullptr) {
          continue;
        }
        // Calculate the position and color of the block
        SDL_Rect block_rect = {col * SCENE_BLOCK_SIZE, row * SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE};
        SDL_Texture *block_texture = SDL_CreateTexture(renderer,
                                                       SDL_PIXELFORMAT_RGBA8888,
                                                       SDL_TEXTUREACCESS_TARGET,
                                                       SCENE_BLOCK_SIZE,
                                                       SCENE_BLOCK_SIZE);

        // Render the BMP texture to the block texture using SDL
        SDL_SetRenderTarget(renderer, block_texture);
        SDL_Rect frame_rect = {0, 0, frame_width, frame_height};
        SDL_Rect dest_rect = {0, 0, SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE};
        SDL_RenderCopy(renderer, mTexture, &frame_rect, nullptr);
        SDL_SetRenderTarget(renderer, nullptr);

        // Store the block texture in the array
        scene_textures[block_index] = block_texture;
      }
    }

    ImVec2 block_offset(20, 30);

    // Display the block textures in the ImGUI window
    for (int row = 0; row < SCENE_NUM_ROWS; row++) {
      for (int col = 0; col < SCENE_NUM_COLS; col++) {
        // Calculate the position and size of the block
        SDL_Rect block_rect = {col * SCENE_BLOCK_SIZE, row * SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE};

        ImGui::SetCursorPos(ImVec2(col * SCENE_BLOCK_SIZE + block_offset.x, row * SCENE_BLOCK_SIZE + block_offset.y));
        // Display the block texture in the ImGUI window
        ImGui::Image((void *) (intptr_t) scene_textures[row * SCENE_NUM_COLS + col],
                     ImVec2(SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE));
        // Check if a block was clicked
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
          std::cout << "clicked: " << row * SCENE_NUM_COLS + col << std::endl;
          if (selected_resource_index != -1) {
            std::cout << "replace with resource: " << selected_resource_index << std::endl;
            scene_textures[row * SCENE_NUM_COLS + col] = resource_textures[selected_resource_index];
            ImGui::SetCursorPos(ImVec2(col * SCENE_BLOCK_SIZE + block_offset.x,
                                       row * SCENE_BLOCK_SIZE + block_offset.y));
            ImGui::Image((void *) (intptr_t) resource_textures[selected_resource_index],
                         ImVec2(SCENE_BLOCK_SIZE, SCENE_BLOCK_SIZE));
          }
        }
      }
    }
    // End ImGUI window
    ImGui::End();

    // Rendering
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x,
                       io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(
        renderer, (Uint8) (clear_color.x * 255), (Uint8) (clear_color.y * 255),
        (Uint8) (clear_color.z * 255), (Uint8) (clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
