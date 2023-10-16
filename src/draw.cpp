#include <cstdio>
#include "draw.hpp"

static void set_up_bit_map(BITMAPINFOHEADER *bmih, BITMAPINFO *dbmi, PixMapHeader *header) {
  // set up bit map
  bmih->biSize = sizeof(*bmih);
  bmih->biWidth = header->width;
  bmih->biHeight = -header->height;
  bmih->biPlanes = 1;
  bmih->biBitCount = 8 * 3;
  bmih->biCompression = BI_RGB;
  bmih->biSizeImage = 0;
  bmih->biXPelsPerMeter = 10;
  bmih->biYPelsPerMeter = 10;
  bmih->biClrUsed = 0;
  bmih->biClrImportant = 0;

  dbmi->bmiHeader = *bmih;
  dbmi->bmiColors->rgbBlue = 0;
  dbmi->bmiColors->rgbGreen = 0;
  dbmi->bmiColors->rgbRed = 0;
  dbmi->bmiColors->rgbReserved = 0;
}

void draw_pix_map_image(HWND hwnd, PixMapImage *image) {
  if (image == NULL) {
    return;
  }

  PAINTSTRUCT ps;
  RECT r;
  GetClientRect(hwnd, &r);
  if (r.bottom == 0) {
    return;
  }

  // set up bit map
  BITMAPINFOHEADER bmih = {0};
  BITMAPINFO dbmi = {0};
  PixMapHeader *header = (PixMapHeader *) image;
  set_up_bit_map(&bmih, &dbmi, header);
  void *bits = (void *) &(image->pixels[0]);

  HDC hdc = BeginPaint(hwnd, &ps);
  HBITMAP hBitmap = CreateDIBSection(hdc, &dbmi, DIB_RGB_COLORS, &bits, NULL, 0);
  if (hBitmap == NULL) {
    printf("[-] Failed to create bitmap. Error code: %lu", GetLastError());
    return;
  }
  memcpy(bits, image->pixels, header->width * header->height * 3);

  // calculate destination size
  int windowWidth = r.right - r.left;
  int windowHeight = r.bottom - r.top - 50;
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

  EndPaint(hwnd, &ps);

  DeleteObject(hBitmap);
}
