#include "buffer.hpp"
#include "buffer_layout.hpp"
#include "defines.hpp"
#include "image_utils.hpp"
#include "sdl_imgui_gl.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include <backends/imgui_impl_sdl3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <print>

namespace {

void APIENTRY debug_output(GLenum source, GLenum type, unsigned int id,
                           GLenum severity, GLsizei length, const char *message,
                           const void *userParam);
static bool process_event(SDL_Event * /*e*/);

f32 square_wave(f32 time, f32 freq, f32 cycles, f32 harmonic);

f32 vertices[] = {
    /*vert*/ 0.5F,    0.5F,  0.0F, /*color*/ 1.0F, 0.5F, 0.8F, 1.0F,
    /*TexCord*/ 1.0F, 1.0F,
    /*vert*/ 0.5F,    -0.5F, 0.0F, /*color*/ 0.8F, 1.0F, 0.5F, 1.0F,
    /*TexCord*/ 1.0F, 0.0F,
    /*vert*/ -0.5F,   -0.5F, 0.0F, /*color*/ 0.5F, 0.8F, 1.0F, 1.0F,
    /*TexCord*/ 0.0F, 0.0F,
    /*vert*/ -0.5F,   0.5F,  0.0F, /*color*/ 1.0F, 0.8F, 0.5F, 1.0F,
    /*TexCord*/ 0.0F, 1.0F,
};

u32 indecies[] = {0, 1, 3, 1, 2, 3};
} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto image_data = image_load("wall.jpg");

  auto ctx = init_sdl("learning_opengl", 1280, 720);
  init_gl(&ctx, true);
  init_audio(&ctx);
  init_imgui(&ctx);
  gl_debug_function(debug_output);

  bool done = false;

  VertexArray vao = {};
  vao.create();

  VertexBuffer vbo = {};
  vbo.create(sizeof(vertices), vertices);
  vbo.layout = {{.type = DataType::Float3, .name = "apos"},
                {.type = DataType::Float4, .name = "aColor"},
                {.type = DataType::Float2, .name = "aTexCord"}};

  IndexBuffer ibo = {};
  ibo.create(6, indecies);

  vao.vbo = std::move(vbo);
  vao.ibo = ibo;

  vao.process_buffers();

  Shader shader = {};
  shader.load_from_file("vertex.vert", "fragment.frag");

  Texture tex = {};
  tex.create();
  tex.update(image_data);

  while (!done) {
    SDL_Event e = {};

    done = process_event(&e);
    start_frame();

    tex.bind();
    shader.use();
    vao.bind();

    auto render = [&]() -> void {
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao.ibo.count),
                     GL_UNSIGNED_INT, nullptr);
    };

    flush_frame(&ctx, render);
  }

  vao.destroy();
  shader.destroy();
  tex.destroy();

  destroy_image(image_data);
  deinit_audio(&ctx);
  deinit_imgui();
  deinit_sdl(&ctx);
  return 0;
}

namespace {
    // auto current_sine_sample = 0.0F;
    // auto freq = 440.0F;
    // auto harmonic = 20.0F;
    // auto cycles = 0.5F;
    // ImGui::Begin("Audio");
    // static f32 samples[512];
    //
    // if (SDL_GetAudioStreamQueued(ctx.audio_stream) < (2048 * 2)) {
    //   for (f32 &sample : samples) {
    //     sample = square_wave(current_sine_sample, freq, cycles, harmonic);
    //     current_sine_sample++;
    //   }
    //   SDL_PutAudioStreamData(ctx.audio_stream, samples, sizeof(samples));
    // }
    //
    // ImGui::PlotLines("WaveForm", samples, IM_ARRAYSIZE(samples), 0, nullptr,
    //                  -1.0F, 1.0F, {0.0F, 80.0F});
    // ImGui::SliderFloat("Frequency", &freq, 1.0F, 1000.0F);
    // ImGui::SliderFloat("Harmonics", &harmonic, 1.0F, 100.0F);
    // ImGui::SliderFloat("Cycles", &cycles, 0.5F, 100.0F);
    // ImGui::End();
f32 square_wave(f32 time, f32 freq, f32 cycles, f32 harmonic) {
  f32 phase = cycles * 2.0F * SDL_PI_F;
  f32 wave1 = 0;
  f32 wave2 = 0;

  for (f32 n = 1; n < harmonic; n++) {
    f32 sine = n * freq * 2.0F * SDL_PI_F * time / 44100.0F;
    wave1 += SDL_sinf(sine) / n;
    wave2 += SDL_sinf(sine - phase * n) / n;
  }
  return (2.0F / SDL_PI_F) * (wave1 - wave2);
}

bool process_event(SDL_Event *e) {
  while (SDL_PollEvent(e)) {
    ImGui_ImplSDL3_ProcessEvent(e);
    switch (e->type) {
    case SDL_EVENT_QUIT: {
      return true;
      break;
    }
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_KEY_DOWN: {
      if (e->key.down && (e->key.scancode == SDL_SCANCODE_ESCAPE)) {
        SDL_Event q = {.type = SDL_EVENT_QUIT};
        SDL_PushEvent(&q);
      }
      break;
    }
    case SDL_EVENT_WINDOW_RESIZED: {
      glViewport(0, 0, e->window.data1, e->window.data2);
      break;
    }
    default:
      break;
    }
  }
  return false;
}

void APIENTRY debug_output(GLenum source, GLenum type, unsigned int id,
                           GLenum severity, [[maybe_unused]] GLsizei length,
                           const char *message,
                           [[maybe_unused]] const void *userParam) {
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) {
    return;
  }

  std::print("{:-^50}\n", "-");

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    std::print("Source: API\n");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::print("Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::print("Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::print("Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::print("Source: Application\n");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    std::print("Source: Other\n");
    break;
  default:
    std::print("Unknown Source Type\n");
    break;
  }

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::print("Type: Error\n");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::print("Type: Deprecated Behaviour\n");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::print("Type: Undefined Behaviour\n");
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    std::print("Type: Portability\n");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::print("Type: Performance\n");
    break;
  case GL_DEBUG_TYPE_MARKER:
    std::print("Type: Marker\n");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::print("Type: Push Group\n");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::print("Type: Pop Group\n");
    break;
  case GL_DEBUG_TYPE_OTHER:
    std::print("Type: Other\n");
    break;
  default:
    std::print("Unknow Type\n");
    break;
  }

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    std::print("Severity: high\n");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::print("Severity: medium\n");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    std::print("Severity: low\n");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::print("Severity: notification\n");
    break;
  default:
    std::print("Unknown Severity Type\n");
    break;
  }
  std::print("Debug Message ID: [{}]\nDebug Message: {}\n{:-^50}", id, message,
             "-");
}
} // namespace
