#pragma once
#include <glad/glad.h>
#include <string_view>

struct Shader {
  uint32_t id = {};

  void load_from_file(std::string_view vertex, std::string_view fragment);

  void use() const;

  void destroy();

  static uint32_t process_shader(const char *source, GLenum shader_type);
};
