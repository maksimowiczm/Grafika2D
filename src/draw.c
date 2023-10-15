#include <stdio.h>
#include "draw.h"

void draw_pix_map_image(HWND hwnd, PixMapImage *image) {
    PAINTSTRUCT ps;
    RECT r;
    GetClientRect(hwnd, &r);
    if (r.bottom == 0) {
        return;
    }

    int windowWidth = r.right - r.left;
    int windowHeight = r.bottom - r.top - 50;

    HDC hdc = BeginPaint(hwnd, &ps);

    PixMapHeader *header = (PixMapHeader *) image;
    size_t length = header->width * header->height * 3;

    // RGB ==> BGR
    uint8_t *pixels = malloc(length);
    memset(pixels, 0, length);
    for (int i = 2; i < length; i += 3) {
        // r -> b
        pixels[i - 2] = image->pixels[i];
        // r -> b
        pixels[i] = image->pixels[i - 2];

        pixels[i - 1] = image->pixels[i - 1];
    }

    // set up bit map
    BITMAPINFOHEADER bmih = {
            sizeof(BITMAPINFOHEADER),
            header->width,
            -header->height,
            1,
            8 * 3,
            BI_RGB,
            0,
            10,
            10,
            0,
            0
    };

    BITMAPINFO dbmi = {0};
    dbmi.bmiHeader = bmih;
    dbmi.bmiColors->rgbBlue = 0;
    dbmi.bmiColors->rgbGreen = 0;
    dbmi.bmiColors->rgbRed = 0;
    dbmi.bmiColors->rgbReserved = 0;
    void *bits = (void *) &(pixels[0]);

    HBITMAP hBitmap = CreateDIBSection(hdc, &dbmi, DIB_RGB_COLORS, &bits, NULL, 0);
    if (hBitmap == NULL) {
        printf("[-] Failed to create bitmap. Error code: %lu", GetLastError());
        return;
    }

    memcpy(bits, pixels, length);

    // calculate destination size
    int width = header->width, height = header->height;
    double aspect_ratio = (double) width / height;
    if (aspect_ratio > ((double) windowWidth / windowHeight)) {
        width = windowWidth;
        height = width / aspect_ratio;
    } else {
        height = windowHeight;
        width = height * aspect_ratio;
    }

    SetStretchBltMode(hdc, HALFTONE);
    StretchDIBits(hdc,
                  0, 50,
                  width, height,
                  0, 0,
                  header->width, header->height,
                  bits,
                  (const BITMAPINFO *) &bmih,
                  DIB_RGB_COLORS,
                  SRCCOPY);

    free(pixels);
}
