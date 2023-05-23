//#include <windows.h>
//#include <dwmapi.h>
//#include <gdiplus.h>
//#include <strsafe.h>
//
//#include <functional>
//#include <iostream>
//#include <memory>
//#include <string>
//#include <vector>
//
//#pragma comment(lib, "gdiplus.lib")
//
//int CaptureAnImage(HWND hWnd)
//{
//  HDC hdcScreen;
//  HDC hdcWindow;
//  HDC hdcMemDC      = NULL;
//  HBITMAP hbmScreen = NULL;
//  BITMAP bmpScreen;
//  DWORD dwBytesWritten = 0;
//  DWORD dwSizeofDIB    = 0;
//  HANDLE hFile         = NULL;
//  char *lpbitmap       = NULL;
//  HANDLE hDIB          = NULL;
//  DWORD dwBmpSize      = 0;
//
//  // Retrieve the handle to a display device context for the client
//  // area of the window.
//  hdcScreen = GetDC(NULL);
//  hdcWindow = GetDC(hWnd);
//
//  // Create a compatible DC, which is used in a BitBlt from the window DC.
//  hdcMemDC = CreateCompatibleDC(hdcWindow);
//
//  if (!hdcMemDC)
//  {
//    MessageBox(hWnd, TEXT("CreateCompatibleDC has failed"), TEXT("Failed"), MB_OK);
//    goto done;
//  }
//
//  // Get the client area for size calculation.
//  RECT rcClient;
//  GetClientRect(hWnd, &rcClient);
//
//  // This is the best stretch mode.
//  SetStretchBltMode(hdcWindow, HALFTONE);
//
//  // The source DC is the entire screen, and the destination DC is the current window (HWND).
//  if (!StretchBlt(hdcWindow, 0, 0, rcClient.right, rcClient.bottom, hdcScreen, 0, 0, GetSystemMetrics(SM_CXSCREEN),
//                  GetSystemMetrics(SM_CYSCREEN), SRCCOPY))
//  {
//    MessageBox(hWnd, TEXT("StretchBlt has failed"), TEXT("Failed"), MB_OK);
//    goto done;
//  }
//
//  // Create a compatible bitmap from the Window DC.
//  hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
//
//  if (!hbmScreen)
//  {
//    MessageBox(hWnd, TEXT("CreateCompatibleBitmap Failed"), TEXT("Failed"), MB_OK);
//    goto done;
//  }
//
//  // Select the compatible bitmap into the compatible memory DC.
//  SelectObject(hdcMemDC, hbmScreen);
//
//  // Bit block transfer into our compatible memory DC.
//  if (!BitBlt(hdcMemDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hdcWindow, 0, 0, SRCCOPY))
//  {
//    MessageBox(hWnd, TEXT("BitBlt has failed"), TEXT("Failed"), MB_OK);
//    goto done;
//  }
//
//  // Get the BITMAP from the HBITMAP.
//  GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);
//
//  BITMAPFILEHEADER bmfHeader;
//  BITMAPINFOHEADER bi;
//
//  bi.biSize          = sizeof(BITMAPINFOHEADER);
//  bi.biWidth         = bmpScreen.bmWidth;
//  bi.biHeight        = bmpScreen.bmHeight;
//  bi.biPlanes        = 1;
//  bi.biBitCount      = 32;
//  bi.biCompression   = BI_RGB;
//  bi.biSizeImage     = 0;
//  bi.biXPelsPerMeter = 0;
//  bi.biYPelsPerMeter = 0;
//  bi.biClrUsed       = 0;
//  bi.biClrImportant  = 0;
//
//  dwBmpSize          = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
//
//  // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
//  // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
//  // have greater overhead than HeapAlloc.
//  hDIB     = GlobalAlloc(GHND, dwBmpSize);
//  lpbitmap = (char *) GlobalLock(hDIB);
//
//  // Gets the "bits" from the bitmap, and copies them into a buffer
//  // that's pointed to by lpbitmap.
//  GetDIBits(hdcWindow, hbmScreen, 0, (UINT) bmpScreen.bmHeight, lpbitmap, (BITMAPINFO *) &bi, DIB_RGB_COLORS);
//
//  // A file is created, this is where we will save the screen capture.
//  hFile = CreateFile(TEXT("captureqwsx.bmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//  // Add the size of the headers to the size of the bitmap to get the total file size.
//  dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//
//  // Offset to where the actual bitmap bits start.
//  bmfHeader.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + (DWORD) sizeof(BITMAPINFOHEADER);
//
//  // Size of the file.
//  bmfHeader.bfSize = dwSizeofDIB;
//
//  // bfType must always be BM for Bitmaps.
//  bmfHeader.bfType = 0x4D42;// BM.
//
//  WriteFile(hFile, (LPSTR) &bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
//  WriteFile(hFile, (LPSTR) &bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
//  WriteFile(hFile, (LPSTR) lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
//
//  // Unlock and Free the DIB from the heap.
//  GlobalUnlock(hDIB);
//  GlobalFree(hDIB);
//
//  // Close the handle for the file that was created.
//  CloseHandle(hFile);
//
//  // Clean up.
//done:
//  DeleteObject(hbmScreen);
//  DeleteObject(hdcMemDC);
//  ReleaseDC(NULL, hdcScreen);
//  ReleaseDC(hWnd, hdcWindow);
//
//  return 0;
//}
//
////1. ��Ļ��hwndΪNULL,��ʱ��ͼ��������rectָ��
////2. ���ڵ�hwnd��Ϊ��,��ʱ���Ը���API��ȡ���ڵĴ�С
//// Ϊ�˽���Ļ�ʹ��ڽ���ͳһ,���ʹ���˽ṹ��
//struct WindowInfo
//{
//  HWND hwnd;             /* Ϊ�ձ�ʾ��Ļ��ͼ */
//  std::string desc;      // ���ڱ���
//  RECT rect{0, 0, 0, 0}; /* hwnd��Ϊ��ʱ,�˲�����Ч */
//};
//
//// �жϵ�ǰ���ָ���ĸ�����
//HWND GetCurPointedWindow()
//{
//  POINT point;
//  BOOL ret = GetCursorPos(&point);
//  if (ret)
//  {
//    HWND hwnd = WindowFromPoint(point);
//    if (hwnd != nullptr && hwnd != INVALID_HANDLE_VALUE) { return hwnd; }
//  }
//  return nullptr;
//}
//
//class Enumerator
//{
//public:
//  using EnumCallback = std::function<void(const WindowInfo &)>;
//
//  static bool EnumMonitor(EnumCallback callback)
//  {
//    // ����Win32Api������ʾ������
//    return ::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM) &callback);
//  }
//
//  static bool EnumWindow(EnumCallback callback)
//  {
//    // ����Win32Api���д��ڱ���
//    return ::EnumWindows(EnumWindowsProc, (LPARAM) &callback);
//  }
//
//private:
//  static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
//  {
//    //::GetParent��ȡ���п����������ߴ���,���ʹ��GetAncestor��ȡ�����ھ��
//    HWND parent             = ::GetAncestor(hwnd, GA_PARENT);
//    HWND desktop            = ::GetDesktopWindow();// ��ȡ����ľ��
//    TCHAR szTitle[MAX_PATH] = {0};
//    ::GetWindowText(hwnd, szTitle, MAX_PATH);// ��ȡ����
//
//    // �ų������ڲ��������
//    if (parent != nullptr && parent != desktop) return TRUE;
//
//    // �ų�����Ϊ�յ�
//    if (wcscmp(szTitle, L"") == 0) return TRUE;
//
//    // �ų���С������(��Ϊ��ȡ��С�����ڵ����������ǲ��Ե�,���Ҳû�취���н�ͼ�Ȳ���)
//    if (::IsIconic(hwnd)) return TRUE;
//
//    // �ų����ɼ�����,�����������ڵ�������ǿɼ���
//    if (!::IsWindowVisible(hwnd)) return TRUE;
//
//    // �ų����û����εĴ���,�ο�[https://docs.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute]
//    DWORD flag = 0;
//    DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &flag, sizeof(flag));
//    if (flag) return TRUE;
//
//    if (lParam)
//    {
//      WindowInfo wnd_info{hwnd, (LPCSTR) CT2A(szTitle, CP_ACP)};
//      EnumCallback *callback_ptr = reinterpret_cast<EnumCallback *>(lParam);
//      callback_ptr->operator()(wnd_info);
//    }
//    return TRUE;
//  }
//
//  static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
//  {
//    MONITORINFOEX mi;
//    mi.cbSize = sizeof(MONITORINFOEX);
//    GetMonitorInfo(hMonitor, &mi);
//    if (dwData)
//    {
//
//      std::string device_name = (LPCSTR) CT2A(mi.szDevice, CP_ACP);
//      if (mi.dwFlags == MONITORINFOF_PRIMARY) device_name += "(Primary)";// ����ʾ��,�ɸ�����Ҫ���в���
//      WindowInfo wnd_info{nullptr, device_name, mi.rcMonitor};
//
//      EnumCallback *callback = reinterpret_cast<EnumCallback *>(dwData);
//      (*callback)(wnd_info);
//    }
//    return TRUE;
//  }
//};
//
//class WindowCapture
//{
//public:
//  using BitmapPtr = std::shared_ptr<Gdiplus::Bitmap>;
//
//  static BitmapPtr Capture(const WindowInfo &wnd_info)
//  {
//    HDC hWndDC = GetWindowDC(wnd_info.hwnd);
//    RECT capture_rect{0, 0, 0, 0};// ����Ҫ��ȡ������
//    RECT wnd_rect;                // ��������
//    RECT real_rect;               // ��ʵ�Ĵ�������,ʵ����Ҳ���ǰٷְ�׼ȷ
//
//    if (wnd_info.hwnd)
//    {
//      ::GetWindowRect(wnd_info.hwnd, &wnd_rect);
//      DwmGetWindowAttribute(wnd_info.hwnd, DWMWINDOWATTRIBUTE::DWMWA_EXTENDED_FRAME_BOUNDS, &real_rect, sizeof(RECT));
//      int offset_left = real_rect.left - wnd_rect.left;
//      int offset_top  = real_rect.top - wnd_rect.top;
//      capture_rect    = RECT{offset_left, offset_top, real_rect.right - real_rect.left + offset_left,
//                          real_rect.bottom - real_rect.top + offset_top};
//    }
//    else { capture_rect = wnd_info.rect; }
//
//    int width       = capture_rect.right - capture_rect.left;
//    int height      = capture_rect.bottom - capture_rect.top;
//
//    HDC hMemDC      = CreateCompatibleDC(hWndDC);
//    HBITMAP hBitmap = CreateCompatibleBitmap(hWndDC, width, height);
//    SelectObject(hMemDC, hBitmap);
//
//    BitmapPtr bitmap;
//    // ��ȡָ�������rgb����
//    bool ok = BitBlt(hMemDC, 0, 0, width, height, hWndDC, capture_rect.left, capture_rect.top, SRCCOPY);
//    // hBitmap���ǵõ���ͼƬ����,תGDI��Bitmap���б���
//    if (ok) bitmap = std::make_shared<Gdiplus::Bitmap>(hBitmap, nullptr);
//
//    DeleteDC(hWndDC);
//    DeleteDC(hMemDC);
//    DeleteObject(hBitmap);
//
//    return bitmap;
//  }
//};
//
//void run()
//{
//  std::vector<WindowInfo> window_vec;// �������洰����Ϣ
//  // ö����ʾ��
//  Enumerator::EnumMonitor([&window_vec](const WindowInfo &wnd_info) { window_vec.push_back(wnd_info); });
//  // ��������������Ļ����һ��������С
//  if (window_vec.size() > 0)
//  {// Ҳ�ɴ���1,����ֻ��һ����ʾ��ʱ������ʾȫ��ѡ��
//    int width = 0, height = 0;
//    for (const auto &wnd_info: window_vec)
//    {
//      width += wnd_info.rect.right - wnd_info.rect.left;
//      int h = wnd_info.rect.bottom - wnd_info.rect.top;
//      if (h > height) height = h;// �߶ȿ��ܲ�һ��,��Ҫ����ߵ�Ϊ׼
//    }
//    WindowInfo wnd_info{nullptr, "FullScreen", {0, 0, width, height}};
//    window_vec.push_back(wnd_info);
//  }
//  // ö�ٴ���
//  Enumerator::EnumWindow([&window_vec](const WindowInfo &wnd_info) { window_vec.push_back(wnd_info); });
//  // ʾ��: �����ҵ������д���,��ÿһ������ͼ��ָ��·��,�ļ��������,���򲻻��Լ������ļ���
//  int cnt = 1;
//  for (const auto &window: window_vec)
//  {
//    printf("%2d. %s\n", cnt, window.desc.c_str());
//    auto bitmap = WindowCapture::Capture(window);
//
//    if (bitmap) GdiplusUtil::SaveBitmapAsPng(bitmap, std::to_string(cnt) + ".png");
//
//    ++cnt;
//  }
//}

#include "think_core/IOManage.h"
#include <iostream>

//#include <filesystem>
//namespace fs = std::filesystem;

int main()
{
  //  DWORD time_start = GetTickCount();
  //  test1();
  //  std::cout << (GetTickCount() - time_start) << "ms\n";
  //
  //  time_start = GetTickCount();
  //
  //  std::cout << (GetTickCount() - time_start) << "ms\n";

    think::IOManage t = think::IOManage();
    for (size_t i = 0U; i < 1; ++i)
    {
      do {
  
        t.runLoop();
      } while (t.getNodeBufferSize() < 0x100);
      t.stableThink();
    }
  
    fs::path a("123");

  return 0;
}