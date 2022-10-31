#include "ScreenDetectionPlayingField.h"
#include "ITesseract.h"
#include <catch.hpp>

using namespace asw;

namespace {

class MockScreenDetectionPlayingField final : public ITesseract {
  public:
    ~MockScreenDetectionPlayingField() override = default;

    [[nodiscard]] std::vector<TesseractDetection>
    detect([[maybe_unused]] std::experimental::mdspan<
            unsigned char const,
            std::experimental::dextents<std::size_t, 2>> image) const override {
        throw std::runtime_error{"NYI"};
    }
};

}  // namespace
