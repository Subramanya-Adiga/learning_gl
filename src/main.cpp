#include "sdl_imgui_gl.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <print>

static constexpr SDL_FColor clear_color = {
    .r = 0.298F, .g = 0.300F, .b = 0.297F, .a = 1.0F};

static bool process_event(SDL_Event * /*e*/);
void APIENTRY debug_output(GLenum source, GLenum type, unsigned int id,
                           GLenum severity, GLsizei length, const char *message,
                           const void *userParam);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto ctx = init_sdl("learning_opengl", 1280, 720);
  init_gl(&ctx, true);
  gl_debug_function(debug_output);

  bool done = false;

  float vertices[] = {
      0.5F, 0.5F, 0.0F,  1.0F, 0.5F,  0.8F,  0.5f, -0.5F,
      0.0F, 0.8F, 1.0F,  0.5F, -0.5F, -0.5F, 0.0F, 0.5F,
      0.8F, 1.0F, -0.5F, 0.5F, 0.0F,  1.0F,  0.8F, 0.5F,
  };

  unsigned int indecies[] = {0, 2, 6, 2, 4, 6};

  uint32_t vao = {};
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t buffer_id = {};
  glGenBuffers(1, &buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  uint32_t ebo = {};
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader shader = {};
  shader.load_from_file("vertex.vert", "fragment.frag");
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!done) {
    SDL_Event e = {};

    done = process_event(&e);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use_shader();
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SDL_GL_SwapWindow(ctx.window);
  }

  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &buffer_id);
  glDeleteBuffers(1, &ebo);
  shader.delete_shader();

  deinit_sdl(&ctx);
  return 0;
}

static bool process_event(SDL_Event *e) {
  while (SDL_PollEvent(e)) {
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
  std::print("Debug Message ID: [{}] \n Debug Message: {}\n", id, message);
}
