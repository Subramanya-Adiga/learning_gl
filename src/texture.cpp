#include "texture.hpp"
#include "image_utils.hpp"
#include <cassert>
#include <glad/glad.h>

static GLenum image_format_to_gl_format(ImagePixelFormat format) {
  switch (format) {
  case ImagePixelFormat::GREYSCALE:
  case ImagePixelFormat::GREYALPHA:
    return GL_NONE;
  case ImagePixelFormat::RGB:
    return GL_RGB;
  case ImagePixelFormat::RGBA:
    return GL_RGBA8;
    break;
  };
  assert(false);
  return GL_NONE;
}

uint32_t texture_slot_count() {
  int count = {};
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &count);
  return count;
}

void Texture::create() {
  glCreateTextures(GL_TEXTURE_2D, 1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::update(const ImageData &data) const {
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(data.width),
               static_cast<GLsizei>(data.height), 0,
               image_format_to_gl_format(data.format), GL_UNSIGNED_BYTE,
               data.data_pointer);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, id); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::destroy() { glDeleteTextures(1, &id); }
