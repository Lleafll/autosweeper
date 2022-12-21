#pragma once

#include <asw/Cell.h>
#include <experimental/mdspan>
class QString;

namespace aswui {

class CellConstSpanView {
  public:
    virtual ~CellConstSpanView() = default;

  private:
    virtual void set_row_count(int rows) = 0;
    virtual void set_column_count(int columns) = 0;
    virtual void set_cell(int row, int column, QString const& text) = 0;

    friend class CellConstSpanPresenter;
};

class CellConstSpanPresenter final {
  public:
    explicit CellConstSpanPresenter(CellConstSpanView& view);

    void set(asw::CellConstSpan const& cells);

  private:
    CellConstSpanView& view_;
};

}  // namespace aswui