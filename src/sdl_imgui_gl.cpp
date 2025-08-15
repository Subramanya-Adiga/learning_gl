#include "sdl_imgui_gl.hpp"
// #include <backends/imgui_impl_opengl3.h>
// #include <backends/imgui_impl_sdl3.h>
#include <SDL3/SDL_video.h>
#include <print>

SDLContext init_sdl(const char *name, int32_t width, int32_t height) {
  SDLContext ret{};
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS |
                SDL_INIT_GAMEPAD)) {
    ret.initialized = false;
    std::print("SDL Failed To Initialize: {}\n", SDL_GetError());
    return ret;
  }

  ret.window = SDL_CreateWindow(name, width, height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                    SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (ret.window == nullptr) {
    ret.initialized = false;
    std::print("SDL Failed To Create Window: {}\n", SDL_GetError());
    return ret;
  }
  return ret;
}

void init_audio(SDLContext *ctx) {
  SDL_AudioSpec spec = {.format = SDL_AUDIO_F32, .channels = 1, .freq = 44100};
  ctx->audio_stream = SDL_OpenAudioDeviceStream(
      SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
  if (ctx->audio_stream == nullptr) {
    std::print("SDL Failed To Create Audio Stream {}\n", SDL_GetError());
  }
  if (ctx->audio_stream != nullptr) {
    sdl_check(SDL_ResumeAudioStreamDevice(ctx->audio_stream));
  }
}

void init_gl(SDLContext *ctx, bool enable_debug) {
  sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0));
  sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_CORE));
  if (enable_debug) {
    sdl_check(
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG));
  }
  sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
  sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6));
  sdl_check(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
  sdl_check(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
  sdl_check(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));

  ctx->gl_context = SDL_GL_CreateContext(ctx->window);
  if (ctx->gl_context == nullptr) {
    ctx->initialized = false;
    std::print("SDL Failed To Create GL Context: {}\n", SDL_GetError());
    return;
  }

  sdl_check(SDL_GL_MakeCurrent(ctx->window, ctx->gl_context));
  sdl_check(SDL_GL_SetSwapInterval(1));

  // GLAD Load Protocals
  if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
    ctx->initialized = false;
    std::print("GLAD Failed To Initialize\n SDL_ERROR:{}\n", SDL_GetError());
    return;
  } else {
    std::print("SDL Version: {}\nOPENGL Version: {}.{}\nGLSL Version: "
               "{}\nOPENGL Vendor: {}\nOPENGL Rendrer: {}\n",
               SDL_GetVersion(), GLVersion.major, GLVersion.minor,
               std::bit_cast<char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)),
               std::bit_cast<char *>(glGetString(GL_VENDOR)),
               std::bit_cast<char *>(glGetString(GL_RENDERER)));
  }
}

void gl_debug_function(GLDEBUGPROC debug_function) {
  int flags = {};
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_function, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }
}

void sdl_check(bool value) {
  if (!value) {
    std::print("SDL Returned Error {}\n", SDL_GetError());
    return;
  }
}

// void init_imgui(SDLContext *ctx) {
//   IMGUI_CHECKVERSION();
//   ImGui::CreateContext();
//   auto &io = ImGui::GetIO();
//   (void)io;
//   io.ConfigFlags |=
//       ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
//   io.ConfigFlags |=
//       ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
//   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

//   ImGui::StyleColorsDark();

//   auto &styles = ImGui::GetStyle();
//   if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
//     styles.WindowRounding = 0.0F;
//     styles.Colors[ImGuiCol_WindowBg].w = 1.0F;
//   }

//   ImGui_ImplSDL3_InitForOpenGL(ctx->window, ctx->gl_context);
//   ImGui_ImplOpenGL3_Init("#version 130");
// }

// void deinit_imgui() {
//   ImGui_ImplOpenGL3_Shutdown();
//   ImGui_ImplSDL3_Shutdown();
//   ImGui::DestroyContext();
// }

void deinit_audio(SDLContext *ctx) {
  if (ctx->audio_stream != nullptr) {
    SDL_DestroyAudioStream(ctx->audio_stream);
  }
}

void deinit_sdl(SDLContext *ctx) {
  sdl_check(SDL_GL_DestroyContext(ctx->gl_context));
  SDL_DestroyWindow(ctx->window);
  SDL_Quit();
}

// void start_frame() {
//   ImGui_ImplOpenGL3_NewFrame();
//   ImGui_ImplSDL3_NewFrame();
//   ImGui::NewFrame();
// }

// void flush_frame(SDLContext *ctx) {
//   auto &io = ImGui::GetIO();
//   ImGui::Render();

//   glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

//   glClearColor(0.556F, 0.629F, 0.830F, 255.0F);
//   glClear(GL_COLOR_BUFFER_BIT);

//   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//   if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
//     auto *backup_window = SDL_GL_GetCurrentWindow();
//     auto *backup_context = SDL_GL_GetCurrentContext();
//     ImGui::UpdatePlatformWindows();
//     ImGui::RenderPlatformWindowsDefault();
//     SDL_GL_MakeCurrent(backup_window, backup_context);
//   }

//   SDL_GL_SwapWindow(ctx->window);
// }
