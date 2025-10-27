#pragma once
#include "buffer.hpp"
#include "defines.hpp"

class VertexArray {
public:
  u32 id = {};
  VertexBuffer vbo = {};
  IndexBuffer ibo = {};

  void create();
  void process_buffers();

  void bind() const;
  void unbind() const;

  void destroy() const;

private:
  u32 v_buffer_count = {};
};
