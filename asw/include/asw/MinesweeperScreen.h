#pragma once

#include "IScreen.h"

namespace asw {

class MinesweeperScreen final : public IScreen {
  public:
    ~MinesweeperScreen() override = default;

    [[nodiscard]] std::optional<Vector2d<int>> grab() const override;
};

}  // namespace asw