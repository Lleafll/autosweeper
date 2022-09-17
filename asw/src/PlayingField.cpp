#include "PlayingField.h"

namespace asw {

PlayingField::PlayingField(MineField mines)
    : rows_{mines.rows()},
      hidden_{mines.rows() * mines.columns(), Hidden::Yes},
      mines_{std::move(mines)} {}

PlayingField::Cell PlayingField::operator()(
        std::size_t const row,
        std::size_t const column) const {
    if (is_hidden(row, column)) {
        return Cell::Hidden;
    }
    return Cell::Empty;
}
void PlayingField::reveal(std::size_t const row, std::size_t const column) {
    hidden_[row + rows_ * column] = Hidden::No;
}

bool PlayingField::is_hidden(std::size_t const row, std::size_t const column)
        const {
    return hidden_[row + rows_ * column] == Hidden::Yes;
}

}  // namespace asw