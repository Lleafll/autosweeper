#include "PlayingField.h"

namespace asw {

PlayingField::PlayingField(MineField mines) : mines_{std::move(mines)} {}

PlayingField::Cell PlayingField::operator()(std::size_t const row,
                                            std::size_t const column) const {
    return PlayingField::Cell::Hidden;
}

}  // namespace asw