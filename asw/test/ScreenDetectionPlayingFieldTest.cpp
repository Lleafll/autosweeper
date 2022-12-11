#include "ScreenDetectionPlayingField.h"
#include "IScreen.h"
#include "ITesseract.h"
#include "algorithm2d.h"
#include <catch.hpp>

namespace {

class MockTesseract final : public asw::ITesseract {
  public:
    ~MockTesseract() override = default;

    void set_image(
            std::experimental::mdspan<
                    unsigned char const,
                    std::experimental::dextents<size_t, 2>> const&,
            asw::ImageInfo const&) override {
    }

    bool recognize() override {
        return true;
    }

    [[nodiscard]] std::unique_ptr<asw::ITesseractResultIterator>
    get_iterator() override {
        return nullptr;
    }
};

class MockScreen final : public asw::IScreen {
  public:
    ~MockScreen() override = default;

    [[nodiscard]] std::optional<asw::Vector2d<unsigned char>>
    grab() const override {
        return std::nullopt;
    }
};

TEST_CASE("Returns empty detection when nothing could be detected") {
    MockTesseract tesseract;
    MockScreen screen;
    asw::ScreenDetectionPlayingField const field{
            asw::Size{2, 2}, tesseract, screen};
    REQUIRE(
            equals(field.cspan(),
                   asw::Array2d<asw::Cell, 2, 2>(asw::Cell::Hidden).cspan()));
}

}  // namespace
