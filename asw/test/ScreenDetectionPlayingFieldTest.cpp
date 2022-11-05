#include "ScreenDetectionPlayingField.h"
#include "IScreen.h"
#include "ITesseract.h"
#include "algorithm2d.h"
#include <catch.hpp>

using namespace asw;

namespace {

class MockTesseract final : public ITesseract {
  public:
    ~MockTesseract() override = default;

    void set_image(
            std::experimental::mdspan<
                    unsigned char const,
                    std::experimental::dextents<std::size_t, 2>>,
            int,
            int) override {
    }

    void recognize() override {
    }

    [[nodiscard]] std::unique_ptr<ITesseractResultIterator>
    get_iterator() override {
        return nullptr;
    }
};

class MockScreen final : public IScreen {
  public:
    ~MockScreen() override = default;

    [[nodiscard]] std::optional<Vector2d<int>> grab() const override {
        return std::nullopt;
    }
};

TEST_CASE("Returns empty detection when nothing could be detected") {
    MockTesseract tesseract;
    MockScreen screen;
    ScreenDetectionPlayingField field{2, 2, tesseract, screen};
    REQUIRE(equals(field.cspan(), Array2d<Cell, 2, 2>(Cell::Hidden).cspan()));
}

}  // namespace
