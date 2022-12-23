#pragma once

#include "Image.h"
#include <memory>
#include <string_view>

namespace asw {

class Logger {
  public:
    template<class T>
    explicit Logger(T t) : self_{std::make_unique<Model<T>>(std::move(t))} {
    }

    void log_image(std::string_view const message, Image const& image) {
        self_->log_image(message, image);
    }

  private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void
        log_image(std::string_view message, Image const& image) = 0;
    };

    template<typename T>
    struct Model final : Concept {
        explicit Model(T data) : data_{std::move(data)} {
        }

        ~Model() override = default;

        void
        log_image(std::string_view const message, Image const& image) override {
            return data_.log_image(message, image);
        }

      private:
        T data_;
    };

    std::unique_ptr<Concept> self_;
};

class NullLogger {
  public:
    void log_image(std::string_view const, Image const&) {
    }
};

}  // namespace asw