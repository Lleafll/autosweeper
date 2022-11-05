#include "MinesweeperScreen.h"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN
#include <windows.h>

namespace asw {

std::optional<Vector2d<int>> asw::MinesweeperScreen::grab() const {
    auto const hwnd = FindWindow(TEXT("Microsoft Minesweeper"), nullptr);
    if (hwnd == nullptr) {
        return std::nullopt;
    }
    RECT rc{};
    GetClientRect(hwnd, &rc);
    auto const hdcScreen = GetDC(nullptr);
    auto const hdc = CreateCompatibleDC(hdcScreen);
    SIZE const size{rc.right - rc.left, rc.bottom - rc.top};
    auto const hbmp = CreateCompatibleBitmap(hdcScreen, size.cx, size.cy);
    SelectObject(hdc, hbmp);
    PrintWindow(hwnd, hdc, PW_CLIENTONLY);
    BITMAPINFO bitmapinfo{};
    bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapinfo.bmiHeader.biWidth = size.cx;
    bitmapinfo.bmiHeader.biHeight = size.cy;
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biBitCount = 4 * 8;
    bitmapinfo.bmiHeader.biCompression = BI_RGB;
    // bitmapinfo.bmiHeader.biSizeImage, per MSDN, may be set to zero for BI_RGB
    // bitmaps
    auto screen_grab = std::make_optional<Vector2d<int>>(size.cx, size.cy);
    GetDIBits(
            hdc,
            hbmp,
            0,
            size.cy,
            static_cast<LPVOID>(screen_grab->data()),
            &bitmapinfo,
            DIB_RGB_COLORS);
    return screen_grab;
}

}  // namespace asw
