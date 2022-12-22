#pragma once

#include "Image.h"
#include <string_view>

namespace asw {

class Logger {
  public:
    virtual ~Logger() = default;

    virtual void log_image(std::string_view message, Image const& image) = 0;
};

class NullLogger final : public Logger {
  public:
    ~NullLogger() override = default;

    void log_image(std::string_view const, Image const&) override {
    }
};

}  // namespace asw