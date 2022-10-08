#include "Solver.h"
#include "PlayingField.h"
#include "predict_mines.h"

namespace asw {

Solver::Solver(PlayingField& field) : field_{field} {
}
Solver::Result Solver::next() {
    [[maybe_unused]] auto const mines = predict_mines(field_.cspan());
    return Result::Success;
}

}  // namespace asw