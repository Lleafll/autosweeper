#include "MinePredictionsPresenter.h"
#include <QString>
#include <catch.hpp>

using namespace aswui;

namespace {

class MockMinePredictionsView final : public MinePredictionsView {
  public:
    std::optional<int> set_row_count_call = {};
    std::optional<int> set_column_count_call = {};
    std::vector<std::tuple<int, int, QString>> set_cell_calls = {};

    ~MockMinePredictionsView() override = default;

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

TEST_CASE("MockMinePredictionsView::set()") {
    MockMinePredictionsView view;
    MinePredictionsPresenter presenter{view};
    SECTION("resizes the field") {
        presenter.set(asw::PredictionArray<23, 56>{}.cspan());
        REQUIRE(view.set_row_count_call == 23);
        REQUIRE(view.set_column_count_call == 56);
    }
    SECTION("sets up field") {
        using enum asw::Prediction;
        presenter.set(  // clang-format off
                asw::PredictionArray<2, 3>{
                    Safe, Unsafe, Unsafe,
                    Unknown, Unknown, Safe}.cspan()
        );  // clang-format on
        std::ranges::sort(view.set_cell_calls);
        std::vector<std::tuple<int, int, QString>> const expected{
                {0, 0, "✓"},
                {0, 1, "⚠"},
                {0, 2, "⚠"},
                {1, 0, "?"},
                {1, 1, "?"},
                {1, 2, "✓"}};
        REQUIRE(view.set_cell_calls == expected);
    }
    SECTION("sets up field with extended asw::Prediction") {
        using enum asw::Prediction;
        presenter.set(asw::PredictionArray<1, 11>{
                Empty,
                One,
                Two,
                Three,
                Four,
                Five,
                Six,
                Seven,
                Eight,
                Hidden,
                Mine}
                              .cspan());
        std::ranges::sort(view.set_cell_calls);
        std::vector<std::tuple<int, int, QString>> const expected{
                {0, 0, " "},
                {0, 1, "1"},
                {0, 2, "2"},
                {0, 3, "3"},
                {0, 4, "4"},
                {0, 5, "5"},
                {0, 6, "6"},
                {0, 7, "7"},
                {0, 8, "8"},
                {0, 9, "☐"},
                {0, 10, "💣"}};
        REQUIRE(view.set_cell_calls == expected);
    }
}

}  // namespace