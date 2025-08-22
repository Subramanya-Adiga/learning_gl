#pragma once
#include <string_view>

enum class ImagePixelFormat : uint8_t {
  GREYSCALE,
  GREYALPHA,
  RGB,
  RGBA,
};

struct ImageData {
  ImagePixelFormat format = {};
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t size = 0;
  uint8_t *data_pointer = nullptr;
};

[[nodiscard]] ImageData image_load(std::string_view name);

void destroy_image(ImageData &data);
