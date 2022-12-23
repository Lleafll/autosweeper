#pragma once

#include "Image.h"
#include "Logger.h"
#include <di/ptr.h>
#include <optional>
namespace asw {
struct Position;
}  // namespace asw

namespace asw {

class MinesweeperScreen final {
  public:
    explicit MinesweeperScreen(di::ptr<Logger> logger);

    [[nodiscard]] std::optional<Image> grab();
    void click(Position const& position);

  private:
    di::ptr<Logger> logger_;
};

}  // namespace asw