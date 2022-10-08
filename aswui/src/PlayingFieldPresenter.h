#pragma once

#include <asw/Cell.h>
#include <experimental/mdspan>
class QString;

namespace aswui {

class PlayingFieldView {
public:
    virtual ~PlayingFieldView() = default;

private:
    virtual void set_row_count(int rows) = 0;
    virtual void set_column_count(int columns) = 0;
    virtual void set_cell(int row, int column, QString const& text) = 0;

    friend class PlayingFieldPresenter;
};

class PlayingFieldPresenter final {
public:
    explicit PlayingFieldPresenter(PlayingFieldView& view);

    void set(asw::ConstCellSpan const& cells);

private:
    PlayingFieldView& view_;
};

}  // namespace aswui