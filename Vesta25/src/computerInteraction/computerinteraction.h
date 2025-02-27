#pragma once
#include <gdiplus.h>
#include <windows.h>

#include <iostream>
#include <string>

#pragma comment(lib, "Gdiplus.lib")

class ScreenController {
 public:
  // Capture un screenshot et retourne le chemin du fichier
  static std::string getScreenShot() {
    const std::filesystem::path dirPath =
        "C:\\Users\\herve\\testopenAicomputer";

    const std::filesystem::path filePath =
        dirPath /
        std::filesystem::path("screenshot_" + std::to_string(GetTickCount64()) +
                              ".jpg");

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    HDC hScreenDC = GetDC(nullptr);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP hBitmap =
        CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0,
           SRCCOPY);

    CURSORINFO cursorInfo = {sizeof(CURSORINFO)};
    if (GetCursorInfo(&cursorInfo) && (cursorInfo.flags == CURSOR_SHOWING)) {
      DrawIconEx(hMemoryDC, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y,
                 cursorInfo.hCursor, 0, 0, 0, nullptr, DI_NORMAL);
    }

    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromHBITMAP(hBitmap, nullptr);
    CLSID clsid;
    GetEncoderClsid(L"image/jpeg", &clsid);
    bitmap->Save(filePath.native().c_str(), &clsid, nullptr);

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(nullptr, hScreenDC);
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return filePath.string();
  }

  // Place la souris à la position spécifiée
  static void setMousePosition(int x, int y) { SetCursorPos(x, y); }

  // Move the mousse up
  static void moveMouseUp(int distance) {
    POINT point;
    if (GetCursorPos(&point)) {
      SetCursorPos(point.x, point.y - distance);
    }
  }

  // Move the mousse down
  static void moveMouseDown(int distance) {
    POINT point;
    if (GetCursorPos(&point)) {
      SetCursorPos(point.x, point.y + distance);
    }
  }

  // Move the mousse left
  static void moveMouseLeft(int distance) {
    POINT point;
    if (GetCursorPos(&point)) {
      SetCursorPos(point.x - distance, point.y);
    }
  }

  // Move the mousse right
  static void moveMouseRight(int distance) {
    POINT point;
    if (GetCursorPos(&point)) {
      SetCursorPos(point.x + distance, point.y);
    }
  }

  // Récupère la position actuelle de la souris
  static std::pair<int, int> getMousePosition() {
    POINT point;
    if (GetCursorPos(&point)) {
      return {point.x, point.y};
    }
    return {-1, -1};  // Valeur d'erreur si la récupération échoue
  }

   // Place la souris à la position spécifiée
  static void setMousePositionPercent(int x, int y) { 
      int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetCursorPos(x * screenWidth / 100, y * screenHeight / 100);
  }

  // Hold click
  static void holdClick(bool left) {
    mouse_event(left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, 0, 0, 0,
                0);
  }

  // Release click
  static void releaseClick(bool left) {
    mouse_event(left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
  }

  // Simule un clic de souris
  static void click(bool left) {
    mouse_event(left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, 0, 0, 0,
                0);
    mouse_event(left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
  }

  // Simule un double-clic de souris
  static void doubleClick(bool left) {
    click(left);
    Sleep(50);  // Petite pause entre les clics
    click(left);
  }

  // Simule la saisie d'une chaîne de caractères au clavier
  static void write(const std::wstring& text) {
    for (WCHAR c : text) {
      SHORT vk = VkKeyScan(c);
      keybd_event((BYTE)vk, 0, 0, 0);
      keybd_event((BYTE)vk, 0, KEYEVENTF_KEYUP, 0);
    }
  }

  // Simule l'appui sur la touche Entrée
  static void enter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
  }

  // Simule l'appui sur la touche Échap
  static void escape() {
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
  }

  // Simule l'appui sur la touche Windows
  static void windows() {
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
  }

 private:
  static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;
    UINT size = 0;
    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;

    Gdiplus::ImageCodecInfo* pImageCodecInfo =
        (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == nullptr) return -1;

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; ++j) {
      if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
        *pClsid = pImageCodecInfo[j].Clsid;
        free(pImageCodecInfo);
        return j;
      }
    }
    free(pImageCodecInfo);
    return -1;
  }
};
