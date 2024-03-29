#define CATCH_CONFIG_ENABLE_OPTIONAL_STRINGMAKER
#include "ImageMatchingPlayingField.h"
#include "algorithm2d.h"
#include <catch.hpp>

using namespace asw;

namespace {

class MockScreen final {
  public:
    MockScreen() = default;

    explicit MockScreen(
            Image image,
            std::optional<Position>* const click_call = nullptr)
        : image_{std::move(image)},
          click_call_{click_call} {
    }

    [[nodiscard]] std::optional<Image> grab() {
        return image_;
    }

    void click(Position const& position) {
        if (click_call_ != nullptr) {
            *click_call_ = position;
        }
    }

  private:
    std::optional<Image> image_ = std::nullopt;
    std::optional<Position>* click_call_ = nullptr;
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
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField const field{
            MockScreen{}, MockMatcher{matcher_call}, 1};
    REQUIRE_FALSE(matcher_call.has_value());
    SECTION("rows") {
        REQUIRE(field.rows() == 0);
    }
    SECTION("columns") {
        REQUIRE(field.columns() == 0);
    }
    SECTION("cspan") {
        REQUIRE(field.cspan().size() == 0);
    }
}

TEST_CASE("ImageMatchingPlayingField when a valid image can be detected") {
    Image const image{{2, 1}, {{0, 1, 2}, {3, 4, 5}}};
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField const field{
            MockScreen{image},
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
    SECTION("cspan") {
        REQUIRE(equals(
                field.cspan(),
                Array2d<asw::Cell, 3, 1>{Cell::One, Cell::Two, Cell::Three}));
    }
    SECTION("operator()") {
        REQUIRE(field(0, 0) == Cell::One);
        REQUIRE(field(0, 1) == Cell::Two);
        REQUIRE(field(0, 2) == Cell::Three);
    }
}

TEST_CASE("ImageMatchingPlayingField when there are not matches") {
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField const field{
            MockScreen{Image{}}, MockMatcher{matcher_call, {}}, 1};
    REQUIRE(field.rows() == 0);
    REQUIRE(field.columns() == 0);
}

TEST_CASE("reveal") {
    std::optional<Position> click_call;
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField field{
            MockScreen{Image{}, &click_call},
            MockMatcher{
                    matcher_call,
                    {{{2, 2}, Cell::One},
                     {{6, 2}, Cell::Two},
                     {{10, 2}, Cell::Three}}},
            4};
    field.reveal({0, 0});
    REQUIRE(click_call == Position{2, 2});
    field.reveal({0, 1});
    REQUIRE(click_call == Position{6, 2});
    field.reveal({0, 2});
    REQUIRE(click_call == Position{10, 2});
}

}  // namespace
