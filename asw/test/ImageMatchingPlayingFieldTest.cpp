#include "ImageMatchingPlayingField.h"
#include "IScreen.h"
#include "algorithm2d.h"
#include <catch.hpp>

using namespace asw;

namespace {

class MockScreen final : public IScreen {
  public:
    MockScreen() = default;
    explicit MockScreen(Image image) : image_{std::move(image)} {
    }

    ~MockScreen() override = default;

    [[nodiscard]] std::optional<Image> grab() const override {
        return image_;
    }

  private:
    std::optional<Image> image_ = std::nullopt;
};

struct MockMatcher final {
    std::optional<Image>& call;
    std::vector<Match> match = {};

    std::vector<Match> operator()(ImageConstSpan const& image) const {
        call = Image{
                Size{image.extent(0), image.extent(1)},
                std::vector<Color>{image.data(), image.data() + image.size()}};
        return match;
    }
};

TEST_CASE("ImageMatchingPlayingField when no image can be detected") {
    MockScreen screen_;
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField const field{
            screen_, MockMatcher{matcher_call}, 1};
    REQUIRE_FALSE(matcher_call.has_value());
    SECTION("rows") {
        REQUIRE(field.rows() == 0);
    }
    SECTION("columns") {
        REQUIRE(field.columns() == 0);
    }
    SECTION("mine_count") {
        REQUIRE(field.mine_count() == 0);
    }
    SECTION("cspan") {
        REQUIRE(field.cspan().size() == 0);
    }
}

TEST_CASE("ImageMatchingPlayingField when a valid image can be detected") {
    Image const image{{2, 1}, {{0, 1, 2}, {3, 4, 5}}};
    MockScreen screen_{image};
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField field{
            screen_,
            MockMatcher{
                    matcher_call,
                    {{{2, 2}, Cell::One},
                     {{6, 2}, Cell::Two},
                     {{10, 2}, Cell::Three}}},
            4};
    REQUIRE(matcher_call.value() == image);
    SECTION("rows") {
        REQUIRE(field.rows() == 3);
    }
    SECTION("columns") {
        REQUIRE(field.columns() == 1);
    }
    SECTION("mine_count") {
        REQUIRE(field.mine_count() == 0);
    }
    SECTION("cspan") {
        REQUIRE(equals(
                field.cspan(),
                Array2d<asw::Cell, 3, 1>{Cell::One, Cell::Two, Cell::Three}
                        .cspan()));
    }
    SECTION("operator()") {
        REQUIRE(field(0, 0) == Cell::One);
        REQUIRE(field(0, 1) == Cell::Two);
        REQUIRE(field(0, 2) == Cell::Three);
    }
}

}  // namespace
