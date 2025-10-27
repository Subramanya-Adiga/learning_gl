#pragma once
#include <initializer_list>
#include "defines.hpp"

enum class DataType : u8 {
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

struct BufferElement {
  DataType type = {};
  bool normalize = false;
  std::string name;
  u32 size = {};
  u32 component_count = {};
  u32 offset = {};
  u32 gl_type = {};
};

struct BufferLayout {
  u32 stride = 0;
  std::vector<BufferElement> elements;

  BufferLayout(std::initializer_list<BufferElement> elems);
};
