#pragma once

#include <asw/Cell.h>
#include <experimental/mdspan>
class QString;

namespace aswui {

class ConstCellSpanView {
public:
    virtual ~ConstCellSpanView() = default;

private:
    virtual void set_row_count(int rows) = 0;
    virtual void set_column_count(int columns) = 0;
    virtual void set_cell(int row, int column, QString const& text) = 0;

    friend class ConstCellSpanPresenter;
};

class ConstCellSpanPresenter final {
public:
    explicit ConstCellSpanPresenter(ConstCellSpanView& view);

    void set(asw::ConstCellSpan const& cells);

private:
    ConstCellSpanView& view_;
};

}  // namespace aswui