#include "netpbm/PixMap.h"

void netpbm_convert_RGB_BGR(PixMapImage *image) {
    PixMapHeader *header = (PixMapHeader *) image;
    size_t length = header->width * header->height * 3;

    for (int i = 2; i < length; i += 3) {
        uint8_t temp = image->pixels[i];

        // r -> b
        image->pixels[i] = image->pixels[i - 2];

        // r -> b
        image->pixels[i - 2] = temp;
    }
}
