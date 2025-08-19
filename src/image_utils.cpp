#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "image_utils.hpp"
#include "file_utils.hpp"
#include <stb_image.h>

ImageData image_load(std::string_view name) {
  auto image_file = file_handle(fopen(name.data(), "rb"),
                                [](FILE *file) { (void)fclose(file); });
  auto image_size = file_handle_helper::file_size(image_file);
  auto image_buffer = file_handle_helper::read_vec_u8(
      image_file, static_cast<uint32_t>(image_size));

  int x = 0;
  int y = 0;
  int n = 0;

  if (stbi_info_from_memory(image_buffer.data(), static_cast<int>(image_size),
                            &x, &y, &n) == 0) {
    std::print("STB Returned Error: {}\n", stbi_failure_reason());
  }

  stbi_set_flip_vertically_on_load(1);
  auto *data = stbi_load_from_memory(
      image_buffer.data(), static_cast<int>(image_size), &x, &y, &n, 0);
  if (data == nullptr) {
    std::print("STB Returned Error: {}\n", stbi_failure_reason());
    return {};
  }

  ImagePixelFormat format = {};

  switch (n) {
  case 1:
    format = ImagePixelFormat::GREYSCALE;
    break;
  case 2:
    format = ImagePixelFormat::GREYALPHA;
    break;
  case 3:
    format = ImagePixelFormat::RGB;
    break;
  case 4:
    format = ImagePixelFormat::RGBA;
    break;
  default:
    assert(false);
    break;
  }

  return {
      .format = format,
      .data_pointer = data,
      .width = static_cast<uint32_t>(x),
      .height = static_cast<uint32_t>(y),
      .size = static_cast<uint32_t>(image_size),
  };
}

void destroy_image(ImageData &data) {
  stbi_image_free(data.data_pointer);
  data = {};
}
