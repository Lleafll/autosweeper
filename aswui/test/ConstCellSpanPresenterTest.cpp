#define CATCH_CONFIG_ENABLE_OPTIONAL_STRINGMAKER
#include "ConstCellSpanPresenter.h"
#include "StringMaker.h"
#include <QString>
#include <catch.hpp>

using namespace aswui;

namespace {

class MockConstCellSpanView final : public ConstCellSpanView {
  public:
    ~MockConstCellSpanView() override = default;

    std::optional<std::size_t> set_row_count_call = {};
    std::optional<std::size_t> set_column_count_call = {};
    std::vector<std::tuple<int, int, QString>> set_cell_calls = {};

  private:
    void set_row_count(int const rows) override {
        set_row_count_call = rows;
    }

    void set_column_count(int const columns) override {
        set_column_count_call = columns;
    }

    void
    set_cell(int const row, int const column, QString const& text) override {
        set_cell_calls.emplace_back(row, column, text);
    }
};

TEST_CASE("ConstCellSpanPresenter") {
    MockConstCellSpanView view{};
    ConstCellSpanPresenter presenter{view};
    SECTION("set() correctly sets up row and column count") {
        presenter.set(asw::Array2d<asw::Cell, 12, 34>{}.cspan());
        REQUIRE(view.set_row_count_call == 12);
        REQUIRE(view.set_column_count_call == 34);
    }
    SECTION("set() correctly sets cells") {
        using enum asw::Cell;
        presenter.set(asw::Array2d<asw::Cell, 3, 4>{// clang-format off
                Empty, One, Two, Three,
                Four, Five, Six, Seven,
                Eight, Hidden, Mine, Empty}  // clang-format on
                              .cspan());
        std::ranges::sort(view.set_cell_calls);
        REQUIRE(view.set_cell_calls ==
                std::vector<std::tuple<int, int, QString>>{
                        {0, 0, ""},
                        {0, 1, "1"},
                        {0, 2, "2"},
                        {0, 3, "3"},
                        {1, 0, "4"},
                        {1, 1, "5"},
                        {1, 2, "6"},
                        {1, 3, "7"},
                        {2, 0, "8"},
                        {2, 1, "☐"},
                        {2, 2, "💣"},
                        {2, 3, ""},
                });
    }
}

}  // namespace