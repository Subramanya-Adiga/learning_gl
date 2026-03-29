#include "shader.hpp"
#include "file_utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <string_view>

void Shader::load_from_file(std::string_view vertex,
                            std::string_view fragment) {
  auto vert_file = file_handle(fopen(vertex.data(), "r"),
                               [](FILE *fhandle) { fclose(fhandle); });
  auto vert_size = file_handle_helper::file_size(vert_file);
  auto vert_source = file_handle_helper::read_string(vert_file, vert_size);
  if (vert_source.empty()) {
    std::print("Vertex Shader Source File Is Empty\nFileName: {}\n", vertex);
  }
  const auto *vert_p = vert_source.c_str();
  auto vert_shader = process_shader(vert_p, GL_VERTEX_SHADER);

  auto frag_file = file_handle(fopen(fragment.data(), "r"),
                               [](FILE *fhandle) { fclose(fhandle); });
  auto frag_size = file_handle_helper::file_size(frag_file);
  auto frag_source = file_handle_helper::read_string(frag_file, frag_size);
  if (frag_source.empty()) {
    std::print("Fragment Shader Source File Is Empty\nFileName: {}\n",
               fragment);
  }
  const auto *frag_p = frag_source.c_str();
  auto frag_shader = process_shader(frag_p, GL_FRAGMENT_SHADER);

  id = glCreateProgram();
  glAttachShader(id, vert_shader);
  glAttachShader(id, frag_shader);
  glLinkProgram(id);

  int success = {};
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (success == 0) {
    int max_length = {};
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<char> err_log(max_length);
    glGetProgramInfoLog(id, max_length, &max_length, err_log.data());
    std::print("{:-^50}\nLinkerError\nLinkError\n{}\n", "-", err_log.data());

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glDeleteShader(id);

    id = 0;
    vert_shader = 0;
    frag_shader = 0;
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

void Shader::use() const { glUseProgram(id); }

void Shader::destroy() {
  glDeleteProgram(id);
  id = 0;
}

void Shader::uploadUniformInt(const char *name, u32 value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform1i(loc, static_cast<GLint>(value));
}

void Shader::uploadUniformIntArray(const char *name,
                                   std::span<int> value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform1iv(loc, static_cast<GLsizei>(value.size()), value.data());
}

void Shader::uploadUniformFloat(const char *name, f32 value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform1f(loc, value);
}

void Shader::uploadUniformFloat2(const char *name,
                                 const glm::vec2 &value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform2f(loc, value.x, value.y);
}

void Shader::uploadUniformFloat3(const char *name,
                                 const glm::vec3 &value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::uploadUniformFloat4(const char *name,
                                 const glm::vec4 &value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::uploadUniformMat3(const char *name, const glm::mat3 &value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::uploadUniformMat4(const char *name, const glm::mat4 &value) const {
  auto loc = glGetUniformLocation(id, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

u32 Shader::process_shader(const char *source, GLenum shader_type) {
  auto shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success = {};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    int max_length = {};
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<char> err_log(max_length);
    glGetShaderInfoLog(shader, max_length, &max_length, err_log.data());
    std::print("{:-^50}\nCompileError\n{}\n", "-", err_log.data());

    glDeleteShader(shader);
    shader = 0;
  }

  return shader;
}
