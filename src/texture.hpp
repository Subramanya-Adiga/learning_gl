#pragma once
#include "image_utils.hpp"

[[nodiscard]] uint32_t texture_slot_count();

struct Texture {
  uint32_t id = {};

  void create_texture();
  void update_texture(const ImageData &data);

  void bind();
  void unbind();

  void destroy_texture();
};
