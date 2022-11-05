#pragma once

#include "Vector2d.h"
#include <optional>

namespace asw {

class IScreen {
  public:
    virtual ~IScreen() = default;

    [[nodiscard]] virtual std::optional<Vector2d<int>> grab() const = 0;
};

}  // namespace asw