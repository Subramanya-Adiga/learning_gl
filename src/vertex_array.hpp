#pragma once
#include "buffer.hpp"

class VertexArray {
public:
  uint32_t id = {};
  VertexBuffer vbo = {};
  IndexBuffer ibo = {};

  void create();
  void process_buffers();

  void bind();
  void unbind();

  void destroy();

private:
  uint32_t v_buffer_count = {};
};
