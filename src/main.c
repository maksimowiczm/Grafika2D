#include <windows.h>
#include <stdio.h>
#include "netpbm/PixMap.h"
#include "netpbm/PixMapReader.h"
#include "inttypes.h"
#include "draw.h"

//const char *file_path = "testy/ppm-test-01-p3.ppm";
const char *file_path = "testy/ppm-test-02-p3-comments.ppm";
//const char *file_path = "testy/ppm-test-07-p3-big.ppm";
PixMapImage *image = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (image == NULL) {
        image = netpbm_PixMap_read_from_file(file_path);
    }

    switch (msg) {
        case WM_PAINT:
            draw_pix_map_image(hwnd, image);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            free(image);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PTSTR cmdline, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Draw Bitmap";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&wc);

    HWND hMainWindow = CreateWindowW(wc.lpszClassName, (LPCWSTR) L"Images",
                                     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                     100, 100, 1280, 720, NULL, NULL, hInstance, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}
