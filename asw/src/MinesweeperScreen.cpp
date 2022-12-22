#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN
#include "MinesweeperScreen.h"
#include <format>
#include <gsl/narrow>
#include <gsl/util>
#include <ranges>
#include <windows.h>

namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

namespace asw {

namespace {

constexpr int sleep_after_foreground_call_ms = 10;
constexpr int bit_depth = 32;

}  // namespace

MinesweeperScreen::MinesweeperScreen(di::ptr<Logger> logger)
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

std::optional<Image> asw::MinesweeperScreen::grab() const {
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
    BITMAPINFO bitmap_info{0};
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
    for (auto row = 0; row < rows; ++row) {
        for (auto column = 0; column < columns; ++column) {
            auto const& quad =
                    buffer[rows * columns - (row * columns + column) - 1];
            (*screen_grab)(row, columns - column - 1) = {
                    quad.rgbRed, quad.rgbGreen, quad.rgbBlue};
        }
    }
    logger_->log_image("Minesweeper Classic", *screen_grab);
    return screen_grab;
}

}  // namespace asw
