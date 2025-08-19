#pragma once
#include "buffer_layout.hpp"

struct VertexBuffer {
  uint32_t id = {};
  BufferLayout layout = {};

  void create(uint32_t size, float *data);

  void bind() const;
  void unbind() const;

  void destroy() const;
};

struct IndexBuffer {
  uint32_t id = {};
  uint32_t count = {};

  void create(uint32_t count, uint32_t *data);

  void bind() const;
  void unbind() const;

  void destroy() const;
};
