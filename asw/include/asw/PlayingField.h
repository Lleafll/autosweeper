#pragma once

#include "Cell.h"
#include "Position.h"
#include <memory>

namespace asw {

class PlayingField {
  public:
    template<class T>
    PlayingField(T t)
        : self_{std::make_unique<Model<T>>(std::move(t))} {
    }

    [[nodiscard]] size_t rows() const {
        return self_->rows();
    }

    [[nodiscard]] size_t columns() const {
        return self_->columns();
    }

    [[nodiscard]] int mine_count() const {
        return self_->mine_count();
    }

    Cell operator()(size_t const row, size_t const column) const {
        return (*self_)(row, column);
    }

    void reveal(Position const& position) {
        self_->reveal(position);
    }

    [[nodiscard]] CellConstSpan cspan() const {
        return self_->cspan();
    }

    void update() {
        self_->update();
    }

  private:
    struct Concept {
        virtual ~Concept() = default;
        [[nodiscard]] virtual size_t rows() const = 0;
        [[nodiscard]] virtual size_t columns() const = 0;
        [[nodiscard]] virtual int mine_count() const = 0;
        [[nodiscard]] virtual Cell
        operator()(size_t row, size_t column) const = 0;
        virtual void reveal(Position const& position) = 0;
        [[nodiscard]] virtual CellConstSpan cspan() const = 0;
        virtual void update() = 0;
    };

    template<typename T>
    struct Model final : Concept {
        explicit Model(T data) : data_{std::move(data)} {
        }
        ~Model() override = default;
        [[nodiscard]] size_t rows() const override {
            return data_.rows();
        }
        [[nodiscard]] size_t columns() const override {
            return data_.columns();
        }
        [[nodiscard]] int mine_count() const override {
            return data_.mine_count();
        }
        Cell operator()(size_t const row, size_t const column) const override {
            return data_(row, column);
        }
        void reveal(Position const& position) override {
            data_.reveal(position);
        }
        [[nodiscard]] CellConstSpan cspan() const override {
            return data_.cspan();
        }
        void update() override {
            data_.update();
        }

      private:
        T data_;
    };

    std::unique_ptr<Concept> self_;
};

}  // namespace asw
