#pragma once

#include "Cell.h"
#include "Position.h"

namespace asw {

struct Match {
    Position screen_position;
    Cell cell;

    bool operator==(Match const&) const = default;
};

}  // namespace asw