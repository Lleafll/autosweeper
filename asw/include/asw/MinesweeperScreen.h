#pragma once

#include "Image.h"
#include "Logger.h"
#include <optional>
namespace asw {
struct Position;
}  // namespace asw

namespace asw {

class MinesweeperScreen final {
  public:
    explicit MinesweeperScreen(pro::proxy<Logger> logger);

    [[nodiscard]] std::optional<Image> grab();
    void click(Position const& position);

  private:
    pro::proxy<Logger> logger_;
};

}  // namespace asw