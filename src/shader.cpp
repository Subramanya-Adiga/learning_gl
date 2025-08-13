#include "shader.hpp"
#include "file_utils.hpp"
#include <print>
#include <string_view>

void Shader::load_from_file(std::string_view vertex,
                            std::string_view fragment) {
  auto vert_file = file_handle(fopen(vertex.data(), "r"),
                               [](FILE *fhandle) { fclose(fhandle); });
  auto vert_size = file_handle_helper::file_size(vert_file);
  auto vert_source = file_handle_helper::read_string(vert_file, vert_size);
  const auto *vert_p = vert_source.c_str();
  auto vert_shader = process_shader(vert_p, GL_VERTEX_SHADER);

  auto frag_file = file_handle(fopen(fragment.data(), "r"),
                               [](FILE *fhandle) { fclose(fhandle); });
  auto frag_size = file_handle_helper::file_size(frag_file);
  auto frag_source = file_handle_helper::read_string(frag_file, frag_size);
  const auto *frag_p = frag_source.c_str();
  auto frag_shader = process_shader(frag_p, GL_FRAGMENT_SHADER);

  shader_id = glCreateProgram();
  glAttachShader(shader_id, vert_shader);
  glAttachShader(shader_id, frag_shader);
  glLinkProgram(shader_id);

  int success = {};
  glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
  if (success == 0) {
    int max_length = {};
    glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

    std::vector<char> err_log(max_length);
    glGetShaderInfoLog(shader_id, max_length, &max_length, err_log.data());
    std::print("{}\n", err_log.data());

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glDeleteShader(shader_id);
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

uint32_t Shader::process_shader(const char *source, GLenum shader_type) {
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
    std::print("{}\n", err_log.data());

    glDeleteShader(shader);
  }

  return shader;
}
