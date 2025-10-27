#include "buffer.hpp"
#include <glad/glad.h>

void VertexBuffer::create(const u32 size, const f32 *data) {
  glCreateBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::destroy() const { glDeleteBuffers(1, &id); }

void IndexBuffer::create(const u32 count, const u32 *data) {
  this->count = count;
  glCreateBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->count * sizeof(u32), data,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::destroy() const { glDeleteBuffers(1, &id); }
