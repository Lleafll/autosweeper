#pragma once

#include "Image.h"
#include <optional>
namespace asw {
struct Position;
}  // namespace asw

namespace asw {

class IScreen {
  public:
    virtual ~IScreen() = default;

    [[nodiscard]] virtual std::optional<Image> grab() const = 0;

    /**
     * @param position Left-click position relative to image
     */
    virtual void click(Position const& position) = 0;
};

}  // namespace asw