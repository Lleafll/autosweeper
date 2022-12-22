#pragma once

#include "IScreen.h"
#include "Image.h"

namespace asw {

class MinesweeperScreen final : public IScreen {
  public:
    ~MinesweeperScreen() override = default;

    [[nodiscard]] std::optional<Image> grab() const override;
};

}  // namespace asw