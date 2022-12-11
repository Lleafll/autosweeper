#pragma once

#include "IScreen.h"

namespace asw {

class MinesweeperScreen final : public IScreen {
  public:
    ~MinesweeperScreen() override = default;

    [[nodiscard]] std::optional<Vector2d<unsigned char>> grab() const override;
};

}  // namespace asw