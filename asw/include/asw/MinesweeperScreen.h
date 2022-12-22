#pragma once

#include "IScreen.h"
#include "Image.h"
#include "Logger.h"
#include "Position.h"
#include <di/ptr.h>

namespace asw {

class MinesweeperScreen final : public IScreen {
  public:
    explicit MinesweeperScreen(di::ptr<Logger> logger);

    ~MinesweeperScreen() override = default;

    [[nodiscard]] std::optional<Image> grab() override;
    void click(Position const& position) override;

  private:
    di::ptr<Logger> logger_;
};

}  // namespace asw