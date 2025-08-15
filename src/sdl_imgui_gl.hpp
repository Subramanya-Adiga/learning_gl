#pragma once
#include <SDL3/SDL.h>
#include <glad/glad.h>
// #include <imgui.h>

struct SDLContext {
  bool initialized = false;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context = nullptr;
  SDL_AudioStream *audio_stream = nullptr;
};

void sdl_check(bool value);

SDLContext init_sdl(const char *name, int32_t width, int32_t height);

void init_audio(SDLContext *ctx);

void init_gl(SDLContext *ctx, bool enable_debug = false);
void gl_debug_function(GLDEBUGPROC debug_function);

// void init_imgui(SDLContext *ctx);

// void deinit_imgui();

void deinit_sdl(SDLContext *ctx);

void deinit_audio(SDLContext *ctx);

// void start_frame();

// void flush_frame(SDLContext *ctx);
