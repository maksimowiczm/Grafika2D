#include "color_converter/rgb_converter.h"

ColorRgb color_converter_cmyk2rgb(ColorCmyk cmyk) {
  return (ColorRgb) {
      255. * (1. - (double) cmyk.cyan / 100.) * (1. - (double) cmyk.key / 100.),
      255. * (1. - (double) cmyk.magenta / 100.) * (1. - (double) cmyk.key / 100.),
      255. * (1. - (double) cmyk.yellow / 100.) * (1. - (double) cmyk.key / 100.)
  };
}
