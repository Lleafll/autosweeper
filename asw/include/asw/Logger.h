#pragma once

#include "Image.h"
#include <memory>
#include <proxy/proxy.h>
#include <string_view>

namespace asw {

struct LogImage final : pro::dispatch<void(std::string_view, Image const&)> {
    void
    operator()(auto& self, std::string_view const message, Image const& image) {
        self.log_image(message, image);
    }
};

struct Logger final : pro::facade<LogImage> {
    static constexpr auto minimum_copyability = pro::constraint_level::trivial;
};

class NullLogger {
  public:
    void log_image(std::string_view const, Image const&) {
    }
};

}  // namespace asw