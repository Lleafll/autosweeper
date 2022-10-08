#include "PlayingFieldPresenter.h"
#define CATCH_CONFIG_ENABLE_OPTIONAL_STRINGMAKER
#include <catch.hpp>

using namespace aswui;

namespace {

class MockPlayingFieldView final : public PlayingFieldView {
public:
    ~MockPlayingFieldView() override = default;

    std::optional<std::size_t> set_row_count_call = {};
    std::optional<std::size_t> set_column_count_call = {};

private:
    void set_row_count(int const rows) override {
        set_row_count_call = rows;
    }

    void set_column_count(int const columns) override {
        set_column_count_call = columns;
    }
};

TEST_CASE("set()") {
    MockPlayingFieldView view{};
    PlayingFieldPresenter presenter{view};
    SECTION("correctly sets up row and column count") {
        presenter.set(asw::CellArray<12, 34>{}.cspan());
        REQUIRE(view.set_row_count_call == 12);
        REQUIRE(view.set_column_count_call == 34);
    }
}

}  // namespace