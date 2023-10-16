#pragma once

#include <cinttypes>
#include <vector>

enum ImageType {
  PPM,
  OPEN_CV,
};

typedef struct {
  int32_t width;
  int32_t height;
} ImageHeader;

typedef struct {
  ImageHeader header;
  uint8_t **pixels;
  void **to_free;
} Image;

typedef struct {
  enum ImageType imageType;
  Image image;
} Context;

bool context_load_image(Context *context, const char *file_path, enum ImageType type);
