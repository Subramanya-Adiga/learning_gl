#pragma once
#include "image_utils.hpp"

[[nodiscard]] uint32_t texture_slot_count();

struct Texture {
  uint32_t id = {};

  void create();
  void update(const ImageData &data) const;

  void bind() const;
  void unbind() const;

  void destroy();
};
