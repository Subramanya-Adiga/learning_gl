#pragma once
#include "image_utils.hpp"
#include "defines.hpp"

[[nodiscard]] u32 texture_slot_count();

struct Texture {
  u32 id = {};

  void create();
  void update(const ImageData &data) const;

  void bind() const;
  void unbind() const;

  void destroy();
};
