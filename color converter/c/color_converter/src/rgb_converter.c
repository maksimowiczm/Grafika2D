#include "color_converter/cmyk_converter.h"

#define MAX_RGB(red, green, blue) \
(                                 \
red > green ?                     \
red > blue ?                      \
red : blue                        \
:                                 \
green > blue ?                    \
green : blue                      \
)

ColorCmyk color_converter_rgb2cmyk(ColorRgb rgb) {
  double red = (double) rgb.red / 255;
  double green = (double) rgb.green / 255;
  double blue = (double) rgb.blue / 255;

  double key = 1. - MAX_RGB(red, green, blue);
  if (key == 1.) {
    return (ColorCmyk) {0, 0, 0, 100};
  }

  double cyan = (1. - red - key) / (1. - key);
  double magenta = (1. - green - key) / (1. - key);
  double yellow = (1. - blue - key) / (1. - key);

  return (ColorCmyk) {
      cyan * 100,
      magenta * 100,
      yellow * 100,
      key * 100
  };
}
