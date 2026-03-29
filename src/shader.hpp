#pragma once
#include "defines.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string_view>

struct Shader {
  u32 id = {};

  void load_from_file(std::string_view vertex, std::string_view fragment);

  void use() const;

  void destroy();

  void uploadUniformInt(const char *name, u32 value) const;
  void uploadUniformIntArray(const char *name, std::span<int> value) const;

  void uploadUniformFloat(const char *name, f32 value) const;
  void uploadUniformFloat2(const char *name, const glm::vec2 &value) const;
  void uploadUniformFloat3(const char *name, const glm::vec3 &value) const;
  void uploadUniformFloat4(const char *name, const glm::vec4 &value) const;

  void uploadUniformMat3(const char *name, const glm::mat3 &value) const;
  void uploadUniformMat4(const char *name, const glm::mat4 &value) const;

  static u32 process_shader(const char *source, GLenum shader_type);
};
