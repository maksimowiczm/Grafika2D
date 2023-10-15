#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "netpbm/PixMap.h"
#include "netpbm/PixMapReader.h"
#include "inttypes.h"

void DrawPixels(HWND hwnd) {
    PAINTSTRUCT ps;
    RECT r;

    GetClientRect(hwnd, &r);

    if (r.bottom == 0) {
        return;
    }

    HDC hdc = BeginPaint(hwnd, &ps);
    PixMapImage *image = netpbm_PixMap_read_from_file("testy/ppm-test-02-p3-comments.ppm");
//    PixMapImage *image = netpbm_PixMap_read_from_file("testy/ppm-test-07-p3-big.ppm");
//    PixMapImage *image = netpbm_PixMap_read_from_file("testy/ppm-test-01-p3.ppm");

    int length = image->header.width * image->header.height * 3;
    int i = 0;
    int y = -1;
    while (i < length) {
        int x = (i / 3) % image->header.width;
        if(x == 0){
            y++;
        }
        SetPixel(hdc, x, y, RGB(image->pixels[i], image->pixels[i + 1], image->pixels[i + 2]));

        i += 3;
    }

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
            DrawPixels(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rcWnd;
            GetClientRect(hwnd, &rcWnd);
            DrawText(hdc, _T("Hello, world!"), -1, &rcWnd, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hwnd, &ps);
        }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
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

    CreateWindowW(wc.lpszClassName, (LPCWSTR) L"Draw Bitmap",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  280, 220, 1280, 720, NULL, NULL, hInstance, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}
