#pragma once
#include <glad/glad.h>
#include <string_view>
#include "defines.hpp"

struct Shader {
  u32 id = {};

  void load_from_file(std::string_view vertex, std::string_view fragment);

  void use() const;

  void destroy();

  static u32 process_shader(const char *source, GLenum shader_type);
};
