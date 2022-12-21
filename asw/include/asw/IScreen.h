#pragma once

#include "Image.h"
#include <optional>

namespace asw {

class IScreen {
  public:
    virtual ~IScreen() = default;

    [[nodiscard]] virtual std::optional<Image>
    grab() const = 0;
};

}  // namespace asw