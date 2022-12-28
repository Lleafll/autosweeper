#pragma once

#include <QString>
#include <asw/Cell.h>
#include <asw/algorithm2d.h>
#include <experimental/mdspan>
#include <gsl/narrow>
#include <memory>

namespace aswui {

template<class T>
concept CellConstSpanView = requires(T t) {
                                { t.set_row_count(int{}) };
                                { t.set_column_count(int{}) };
                                { t.set_cell(int{}, int{}, QString{}) };
                            };

namespace detail {

QString to_qstring(asw::Cell cell);

}  // namespace detail

template<class T>
class CellConstSpanPresenter final {
  public:
    explicit CellConstSpanPresenter(T& view) : view_{view} {
    }

    void set(asw::CellConstSpan const& cells) {
        view_.set_row_count(gsl::narrow_cast<int>(cells.extent(0)));
        view_.set_column_count(gsl::narrow_cast<int>(cells.extent(1)));
        asw::indexed_for_each(
                cells, [this](asw::Position const& i, asw::Cell const cell) {
                    view_.set_cell(
                            gsl::narrow_cast<int>(i.row),
                            gsl::narrow_cast<int>(i.column),
                            detail::to_qstring(cell));
                });
    }

  private:
    T& view_;
};

}  // namespace aswui