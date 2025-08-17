#include "buffer_layout.hpp"
#include <cassert>
#include <glad/glad.h>

namespace {
struct element_data {
  uint32_t size = {};
  uint32_t comp = {};
  uint32_t gl_type = {};
};

element_data get_data(DataType type) {
  switch (type) {
  case DataType::Float:
    return {.size = 4, .comp = 1, .gl_type = GL_FLOAT};
  case DataType::Float2:
    return {.size = 8, .comp = 2, .gl_type = GL_FLOAT};
  case DataType::Float3:
    return {.size = 12, .comp = 3, .gl_type = GL_FLOAT};
  case DataType::Float4:
    return {.size = 16, .comp = 4, .gl_type = GL_FLOAT};
  case DataType::Mat3:
    return {.size = 36, .comp = 3, .gl_type = GL_FLOAT};
  case DataType::Mat4:
    return {.size = 64, .comp = 4, .gl_type = GL_FLOAT};
  case DataType::Int:
    return {.size = 4, .comp = 1, .gl_type = GL_INT};
  case DataType::Int2:
    return {.size = 8, .comp = 2, .gl_type = GL_INT};
  case DataType::Int3:
    return {.size = 12, .comp = 3, .gl_type = GL_INT};
  case DataType::Int4:
    return {.size = 16, .comp = 4, .gl_type = GL_INT};
  case DataType::Bool:
    return {.size = 1, .comp = 1, .gl_type = GL_BOOL};
  }
  assert(false);
  return {};
}
} // namespace

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elems)
    : elements(elems) {
  uint32_t offset = 0;
  for (auto &&e : elements) {
    auto [s, c, g] = get_data(e.type);
    e.size = s;
    e.component_count = c;
    e.gl_type = g;
    e.offset = offset;
    offset += e.size;
    stride += e.size;
  }
}
