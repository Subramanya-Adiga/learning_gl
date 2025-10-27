#pragma once
#include "defines.hpp"
#include <string_view>

enum class ImagePixelFormat : u8 {
  GREYSCALE,
  GREYALPHA,
  RGB,
  RGBA,
};

struct ImageData {
  ImagePixelFormat format = {};
  u32 width = 0;
  u32 height = 0;
  u32 size = 0;
  u8 *data_pointer = nullptr;
};

[[nodiscard]] ImageData image_load(std::string_view name);

void destroy_image(ImageData &data);
