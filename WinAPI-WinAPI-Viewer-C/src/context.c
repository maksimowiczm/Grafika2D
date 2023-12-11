#include "context.h"

#include "netpbm/PixMapReader.h"
#include "stdlib.h"

bool context_load_image(Context *context, const char *file_path, enum ImageType type) {
  if (type == PPM) {
    PixMapImage *image = netpbm_PixMap_read_from_file(file_path);
    if (image == NULL) {
      return false;
    }
    netpbm_convert_RGB_BGR(image);

    free(context->image);
    context->image = image;

    return true;
  }
}