#include "vertex_array.hpp"
#include <cassert>
#include <glad/glad.h>

void VertexArray::create() { glCreateVertexArrays(1, &id); }

void VertexArray::process_buffers() {
  glBindVertexArray(id);
  vbo.bind();
  for (auto &&layout : vbo.layout.elements) {
    switch (layout.type) {
    case DataType::Float:
    case DataType::Float2:
    case DataType::Float3:
    case DataType::Float4: {
      glEnableVertexAttribArray(v_buffer_count);
      glVertexAttribPointer(
          v_buffer_count, static_cast<GLint>(layout.component_count),
          layout.gl_type, layout.normalize ? GL_TRUE : GL_FALSE,
          static_cast<GLint>(vbo.layout.stride), (const void *)(layout.offset));
      v_buffer_count++;
      break;
    }
    case DataType::Mat3:
    case DataType::Mat4: {
      break;
    }
    case DataType::Int:
    case DataType::Int2:
    case DataType::Int3:
    case DataType::Int4:
    case DataType::Bool: {
      glEnableVertexAttribArray(v_buffer_count);
      glVertexAttribIPointer(
          v_buffer_count, static_cast<GLint>(layout.component_count),
          layout.gl_type, static_cast<GLint>(vbo.layout.stride),
          (const void *)(layout.offset));
      v_buffer_count++;
      break;
    }
    default:
      assert(false);
      break;
    }
  }
  glBindVertexArray(0);
}

void VertexArray::bind() const {
  glBindVertexArray(id);
  vbo.bind();
  ibo.bind();
}

void VertexArray::unbind() const {
  glBindVertexArray(0);
  vbo.unbind();
  ibo.unbind();
}

void VertexArray::destroy() const {
  glDeleteVertexArrays(1, &id);
  vbo.destroy();
  ibo.destroy();
}
