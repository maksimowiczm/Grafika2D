#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "inttypes.h"
#include "draw.h"
#include "context.h"

Context *context;

static bool
handle_button_load_click(HWND hwnd) {
  OPENFILENAME ofn;
  TCHAR szFile[260] = {0};

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = _T("PPM\0*.ppm\0All\0*.*\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn) == TRUE) {
    const char *file_path = ofn.lpstrFile;
    return context_load_image(context, file_path, PPM);
  }

  if (strlen(ofn.lpstrFile) > 0) {
    return false;
  }

  return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_PAINT:draw_pix_map_image(hwnd, context->image);
      break;
    case WM_DESTROY:PostQuitMessage(0);
      return 0;
    case WM_COMMAND: {
      bool loaded = handle_button_load_click(hwnd);

      // redraw after load
      if (loaded) {
        RECT r;
        GetClientRect(hwnd, &r);
        InvalidateRect(hwnd, &r, TRUE);
        break;
      }

      MessageBox(hwnd, "File type is not supported or file is corrupted!", "Error", MB_ICONERROR | MB_OK);
    }
      break;
    default:return DefWindowProcW(hwnd, msg, wParam, lParam);
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PTSTR cmdline, int nCmdShow) {
  Context main_context = {0};
  context = &main_context;

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
  HWND hwndButton = CreateWindow(
      "BUTTON",  // Predefined class; Unicode assumed
      "Load file",      // Button text
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
      0,         // x position
      0,         // y position
      100,        // Button width
      50,        // Button height
      hMainWindow,     // Parent window
      (HMENU) 10000,       // No menu.
      (HINSTANCE) GetWindowLongPtr(hMainWindow, GWLP_HINSTANCE),
      NULL);      // Pointer not needed.

  UpdateWindow(hMainWindow);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  free(main_context.image);
  return (int) msg.wParam;
}
