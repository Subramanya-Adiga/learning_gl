#include "sdl_imgui_gl.hpp"
#include <glad/glad.h>
#include "shader.hpp"
#include <print>

static constexpr SDL_FColor clear_color = {
    .r = 0.298F, .g = 0.300F, .b = 0.297F, .a = 1.0F};

static bool process_event(SDL_Event * /*e*/);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  auto ctx = init_sdl("learning_opengl", 1280, 720);
  init_gl(&ctx);

  bool done = false;

  float vertices[] = {
      0.5F, 0.5F, 0.0F,  1.0F, 0.5F,  0.8F,  
      0.5f, -0.5F, 0.0F, 0.8F, 1.0F,  0.5F, 
      -0.5F, -0.5F, 0.0F, 0.5F, 0.8F, 1.0F, 
      -0.5F, 0.5F, 0.0F,  1.0F,  0.8F, 0.5F,
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

  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader);

  int success;
  char infolog[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, infolog);
    std::print("{}\n", infolog);
  }

  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, infolog);
    std::print("{}\n", infolog);
  }

  auto shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(shader_program, 512, nullptr, infolog);
    std::print("{}\n", infolog);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!done) {
    SDL_Event e = {};

    done = process_event(&e);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    SDL_GL_SwapWindow(ctx.window);
  }

  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &buffer_id);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(shader_program);

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
