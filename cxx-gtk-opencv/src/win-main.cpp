#include "Windows.h"
#include "tchar.h"
#include "image_adapter/imageViewer.hpp"

static bool
handle_button_load_click(HWND hwnd) {
  OPENFILENAME ofn;
  TCHAR szFile[260] = {0};

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = _T("JPEG\0*.jpg\0PPM\0*.ppm\0All\0*.*\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = nullptr;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = nullptr;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn) == TRUE) {
    const char *file_path = ofn.lpstrFile;
    if (strlen(file_path) < 3) {
      return false;
    }

    if (strcmp(&file_path[strlen(file_path) - 3], "ppm") == 0) {
      return ImageViewer::load_image(file_path, ImageLoader::LoaderMethod::PPM);
    }

    return ImageViewer::load_image(file_path, ImageLoader::LoaderMethod::OPEN_CV);
  }

  if (strlen(ofn.lpstrFile) > 0) {
    return false;
  }

  return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_DESTROY:PostQuitMessage(0);
      return 0;
    case WM_COMMAND: {
      if (handle_button_load_click(hwnd)) {
        break;
      }

      MessageBox(hwnd, "File type is not supported or file is corrupted!", "Error", MB_ICONERROR | MB_OK);
      break;
    }
    default:return DefWindowProcW(hwnd, msg, wParam, lParam);
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PTSTR cmdline, int nCmdShow) {
  WNDCLASSW wc = {0};
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpszClassName = L"Draw Bitmap";
  wc.hInstance = hInstance;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc = WndProc;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  RegisterClassW(&wc);

  // Main window
  constexpr int WIDTH = 200, HEIGHT = 200;
  HWND hMainWindow = CreateWindowW(wc.lpszClassName, (LPCWSTR) L"Images",
                                   WS_VISIBLE | WS_OVERLAPPED  | WS_SYSMENU,
                                   100, 100, WIDTH, HEIGHT,
                                   nullptr, nullptr, hInstance, nullptr);

  // Load image button
  HWND hwndButton = CreateWindow("BUTTON", "Load file", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                 0, 0, WIDTH - 15, HEIGHT - 39,
                                 hMainWindow, nullptr,
                                 (HINSTANCE) GetWindowLongPtr(hMainWindow, GWLP_HINSTANCE),
                                 nullptr);

  UpdateWindow(hMainWindow);
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int) msg.wParam;
}
