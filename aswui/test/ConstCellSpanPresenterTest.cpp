#define CATCH_CONFIG_ENABLE_OPTIONAL_STRINGMAKER
#include "CellConstSpanPresenter.h"
#include "StringMaker.h"
#include <QString>
#include <catch.hpp>

using aswui::CellConstSpanPresenter;
using aswui::CellConstSpanView;

namespace {

class MockConstCellSpanView final : public CellConstSpanView {
  public:
    ~MockConstCellSpanView() override = default;

    std::optional<size_t> set_row_count_call = {};
    std::optional<size_t> set_column_count_call = {};
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
    static constexpr int rows = 12;
    static constexpr int columns = 34;
    MockConstCellSpanView view{};
    CellConstSpanPresenter presenter{view};
    SECTION("set() correctly sets up row and column count") {
        presenter.set(asw::Array2d<asw::Cell, rows, columns>{}.cspan());
        REQUIRE(view.set_row_count_call == rows);
        REQUIRE(view.set_column_count_call == columns);
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