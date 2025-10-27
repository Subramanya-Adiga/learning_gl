#pragma once
#include "buffer_layout.hpp"
#include "defines.hpp"

struct VertexBuffer {
  u32 id = {};
  BufferLayout layout = {};

  void create(const u32 size, const f32 *data);

  void bind() const;
  void unbind() const;

  void destroy() const;
};

struct IndexBuffer {
  u32 id = {};
  u32 count = {};

  void create(const u32 count, const u32 *data);

  void bind() const;
  void unbind() const;

  void destroy() const;
};
