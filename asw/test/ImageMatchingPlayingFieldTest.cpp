#include "ImageMatchingPlayingField.h"
#include "IScreen.h"
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
        call =
                Image{Size{image.extent(0), image.extent(1)},
                      std::vector<unsigned char>{
                              image.data(), image.data() + image.size()}};
        return match;
    }
};

TEST_CASE("ImageMatchingPlayingField when no image can be detected") {
    MockScreen screen_;
    std::optional<Image> matcher_call;
    ImageMatchingPlayingField const field{screen_, MockMatcher{matcher_call}};
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

}  // namespace
