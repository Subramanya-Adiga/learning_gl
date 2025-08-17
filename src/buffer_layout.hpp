#pragma once
#include <initializer_list>

enum class DataType : uint8_t {
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
  uint32_t size = {};
  uint32_t component_count = {};
  uint32_t offset = {};
  uint32_t gl_type = {};
};

struct BufferLayout {
  uint32_t stride = 0;
  std::vector<BufferElement> elements;

  BufferLayout(std::initializer_list<BufferElement> elems);
};
