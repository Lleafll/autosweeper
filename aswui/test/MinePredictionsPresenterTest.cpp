#include "MinePredictionsPresenter.h"
#include <catch.hpp>

using namespace aswui;

namespace {

class MockMinePredictionsView final : public MinePredictionsView {
  public:
    std::optional<int> set_row_count_call = {};
    std::optional<int> set_column_count_call = {};

    ~MockMinePredictionsView() override = default;

  private:
    void set_row_count(int const rows) override {
        set_row_count_call = rows;
    }

    void set_column_count(int const columns) override {
        set_column_count_call = columns;
    }
};

TEST_CASE("MockMinePredictionsView::set()") {
    MockMinePredictionsView view;
    MinePredictionsPresenter presenter{view};
    SECTION("correctly resizes the field") {
        presenter.set(23, 56, {});
        REQUIRE(view.set_row_count_call == 23);
        REQUIRE(view.set_column_count_call == 56);
    }
}

}  // namespace