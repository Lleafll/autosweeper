#include "Solver.h"
#include "PlayingField.h"
#include "predict_mines.h"
#include <cassert>

namespace asw {

Solver::Solver(PlayingField& field) : field_{field} {}
Solver::Result Solver::next() {
    [[maybe_unused]] auto const mines = predict_mines(field_);
    return Result::Success;
}

}  // namespace asw