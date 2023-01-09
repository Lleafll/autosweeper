#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN
#include "MinesweeperScreen.h"
#include "Position.h"
#include <format>
#include <gsl/narrow>
#include <windows.h>

namespace asw {

namespace {

constexpr int sleep_after_foreground_call_ms = 100;
constexpr int bit_depth = 32;

}  // namespace

MinesweeperScreen::MinesweeperScreen(pro::proxy<Logger> logger)
    : logger_{std::move(logger)} {
}

namespace {

HBITMAP capture_fullscreen(HDC hdcScreen, HDC hdc, RECT const& rc) {
    auto const width = rc.right - rc.left;
    auto const height = rc.bottom - rc.top;
    auto* const hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
    auto* const previous = SelectObject(hdc, hbmp);
    auto const bOK = BitBlt(
            hdc, 0, 0, width, height, hdcScreen, rc.left, rc.top, SRCCOPY);
    SelectObject(hdc, previous);
    if (bOK == 0) {
        DeleteObject(hbmp);
        throw std::runtime_error{
                std::format("BitBlt failed, error code: {}", GetLastError())};
    }
    return hbmp;
}

}  // namespace

std::optional<Image> asw::MinesweeperScreen::grab() {
    auto* const hwnd = FindWindow(nullptr, "Minesweeper Classic");
    if (hwnd == nullptr) {
        return std::nullopt;
    }
    auto* const hdcScreen = GetDC(HWND_DESKTOP);
    auto const hdc_screen_cleanup =
            gsl::finally([&] { ReleaseDC(HWND_DESKTOP, hdcScreen); });
    auto* const hdc = CreateCompatibleDC(hdcScreen);
    auto const hdc_cleanup = gsl::finally([&] { DeleteDC(hdc); });
    RECT rc{};
    GetWindowRect(hwnd, &rc);
    SetForegroundWindow(hwnd);
    Sleep(sleep_after_foreground_call_ms);
    auto* const bitmap = capture_fullscreen(hdcScreen, hdc, rc);
    auto const bitmap_cleanup = gsl::finally([&] { DeleteObject(bitmap); });
    BITMAPINFO bitmap_info;
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    if (auto const status = GetDIBits(
                hdc, bitmap, 0, 0, nullptr, &bitmap_info, DIB_RGB_COLORS);
        status == 0) {
        throw std::runtime_error{
                std::format("GetDIBits return status: {}", status)};
    }
    bitmap_info.bmiHeader.biBitCount = bit_depth;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    // correct the bottom-up ordering of lines
    bitmap_info.bmiHeader.biHeight = std::abs(bitmap_info.bmiHeader.biHeight);
    std::vector<RGBQUAD> buffer(gsl::narrow_cast<size_t>(
            bitmap_info.bmiHeader.biHeight * bitmap_info.bmiHeader.biWidth));
    auto const status = GetDIBits(
            hdc,
            bitmap,
            0,
            bitmap_info.bmiHeader.biHeight,
            static_cast<LPVOID>(buffer.data()),
            &bitmap_info,
            DIB_RGB_COLORS);
    if (status == 0) {
        throw std::runtime_error{
                std::format("GetDIBits return status: {}", status)};
    }
    auto const rows = gsl::narrow_cast<size_t>(bitmap_info.bmiHeader.biHeight);
    auto const columns =
            gsl::narrow_cast<size_t>(bitmap_info.bmiHeader.biWidth);
    auto screen_grab =
            std::make_optional<Image>(Size{.rows = rows, .columns = columns});
    for (auto row = 0; std::cmp_less(row, rows); ++row) {
        for (auto column = 0; std::cmp_less(column, columns); ++column) {
            auto const& quad =
                    buffer[rows * columns - (row * columns + column) - 1];
            (*screen_grab)(row, columns - column - 1) = {
                    quad.rgbRed, quad.rgbGreen, quad.rgbBlue};
        }
    }
    logger_.invoke<LogImage>("Minesweeper Classic", *screen_grab);
    return screen_grab;
}

void MinesweeperScreen::click(Position const& position) {
    auto* const hwnd = FindWindow(nullptr, "Minesweeper Classic");
    if (hwnd == nullptr) {
        return;
    }
    RECT rc{};
    GetWindowRect(hwnd, &rc);
    POINT cursor_start_position;
    if (GetCursorPos(&cursor_start_position) == 0) {
        throw std::runtime_error(std::to_string(GetLastError()));
    }
    auto const scale_to_point =
            [nScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN),
             nScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN),
             nScreenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN),
             nScreenTop = GetSystemMetrics(SM_YVIRTUALSCREEN)](
                    POINT const& point) -> POINT {
        int const nX = static_cast<int>(
                ((static_cast<double>(point.x) - nScreenLeft) * 65536) /
                        nScreenWidth +
                65536.0 / (nScreenWidth));
        int const nY = static_cast<int>(
                ((static_cast<double>(point.y) - nScreenTop) * 65536) /
                        nScreenHeight +
                65536.0 / (nScreenHeight));
        return {nX, nY};
    };
    auto const click_point = scale_to_point(
            {rc.left +
                     gsl::narrow_cast<long>(  // NOLINT(google-runtime-int)
                             position.row) +
                     1,
             rc.top +
                     gsl::narrow_cast<long>(  // NOLINT(google-runtime-int)
                             position.column) +
                     1});
    auto const return_point = scale_to_point(cursor_start_position);
    std::array<INPUT, 4> mi{};
    mi[0].type = INPUT_MOUSE;
    mi[0].mi.dx  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = click_point.x;
    mi[0].mi.dy  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = click_point.y;
    mi[0].mi.dwFlags  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE;
    mi[1].mi.dwFlags  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK |
              MOUSEEVENTF_LEFTDOWN;
    mi[2].mi.dwFlags  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK |
              MOUSEEVENTF_LEFTUP;
    mi[3].mi.dwFlags  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_MOVE;
    mi[3].mi.dx  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = return_point.x;
    mi[3].mi.dy  // NOLINT(cppcoreguidelines-pro-type-union-access)
            = return_point.y;
    SendInput(gsl::narrow_cast<UINT>(mi.size()), mi.data(), sizeof(mi[0]));
}

}  // namespace asw
