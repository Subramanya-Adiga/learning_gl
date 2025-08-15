#include "buffer.hpp"

void VertexBuffer::create(uint32_t size, float *data) {
  glCreateBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::destroy() { glDeleteBuffers(1, &id); }

void IndexBuffer::create(uint32_t count, uint32_t *data) {
  glCreateBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data,
               GL_STATIC_DRAW);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::destroy() { glDeleteBuffers(1, &id); }
