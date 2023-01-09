#define CATCH_CONFIG_ENABLE_OPTIONAL_STRINGMAKER
#include "ImageMatchingPlayingField.h"
#include "algorithm2d.h"
#include <catch.hpp>

namespace {

class MockScreen final {
  public:
    MockScreen() = default;

    explicit MockScreen(
            asw::Image image,
            std::optional<asw::Position>* const click_call = nullptr)
        : image_{std::move(image)},
          click_call_{click_call} {
    }

    [[nodiscard]] std::optional<asw::Image> grab() {
        return image_;
    }

    void click(asw::Position const& position) {
        if (click_call_ != nullptr) {
            *click_call_ = position;
        }
    }

  private:
    std::optional<asw::Image> image_ = std::nullopt;
    std::optional<asw::Position>* click_call_ = nullptr;
};

struct MockMatcher final {
    std::optional<asw::Image>& call;
    std::vector<asw::Match> match = {};

    std::vector<asw::Match> operator()(asw::ImageConstSpan const& image) const {
        call = asw::Image{
                asw::Size{image.extent(0), image.extent(1)},
                std::vector<asw::Color>{
                        image.data(), image.data() + image.size()}};
        return match;
    }
};

TEST_CASE("ImageMatchingPlayingField when no image can be detected") {
    std::optional<asw::Image> matcher_call;
    asw::ImageMatchingPlayingField const field{
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

TEST_CASE(  // NOLINT(readability-function-cognitive-complexity)
        "ImageMatchingPlayingField when a valid image can be detected") {
    asw::Image const image{{2, 1}, {{0, 1, 2}, {3, 4, 5}}};
    std::optional<asw::Image> matcher_call;
    asw::ImageMatchingPlayingField const field{
            MockScreen{image},
            MockMatcher{
                    matcher_call,
                    {{{2, 2}, asw::Cell::One},
                     {{6, 2}, asw::Cell::Two},
                     {{10, 2}, asw::Cell::Three}}},
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
                asw::Array2d<asw::Cell, 3, 1>{
                        asw::Cell::One, asw::Cell::Two, asw::Cell::Three}));
    }
    SECTION("operator()") {
        REQUIRE(field(0, 0) == asw::Cell::One);
        REQUIRE(field(0, 1) == asw::Cell::Two);
        REQUIRE(field(0, 2) == asw::Cell::Three);
    }
}

TEST_CASE("ImageMatchingPlayingField when there are not matches") {
    std::optional<asw::Image> matcher_call;
    asw::ImageMatchingPlayingField const field{
            MockScreen{asw::Image{}}, MockMatcher{matcher_call, {}}, 1};
    REQUIRE(field.rows() == 0);
    REQUIRE(field.columns() == 0);
}

TEST_CASE("reveal") {
    std::optional<asw::Position> click_call;
    std::optional<asw::Image> matcher_call;
    asw::ImageMatchingPlayingField field{
            MockScreen{asw::Image{}, &click_call},
            MockMatcher{
                    matcher_call,
                    {{{2, 2}, asw::Cell::One},
                     {{6,  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                       2},
                      asw::Cell::Two},
                     {{10,  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
                       2},
                      asw::Cell::Three}}},
            4};
    field.reveal({0, 0});
    REQUIRE(click_call == asw::Position{2, 2});
    field.reveal({0, 1});
    REQUIRE(click_call == asw::Position{6, 2});
    field.reveal({0, 2});
    REQUIRE(click_call == asw::Position{10, 2});
}

}  // namespace
