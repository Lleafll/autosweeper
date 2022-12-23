#pragma once

#include <asw/Cell.h>
#include <experimental/mdspan>
#include <memory>
class QString;

namespace aswui {

class CellConstSpanView {
  public:
    template<class T>
    CellConstSpanView(T t) : self_{std::make_unique<Model<T>>(std::move(t))} {
    }

    void set_row_count(int const rows) {
        self_->set_row_count(rows);
    }

    void set_column_count(int const columns) {
        self_->set_column_count(columns);
    }

    void set_cell(int const row, int const column, QString const& text) {
        self_->set_cell(row, column, text);
    }

  private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void set_row_count(int rows) = 0;
        virtual void set_column_count(int columns) = 0;
        virtual void set_cell(int row, int column, QString const& text) = 0;
    };

    template<class T>
    struct Model final : Concept {
        explicit Model(T data) : data_{std::move(data)} {
        }
        ~Model() override = default;
        void set_row_count(int const rows) override {
            data_.set_row_count(rows);
        }
        void set_column_count(int const columns) override {
            data_.set_column_count(columns);
        }
        void set_cell(int const row, int const column, QString const& text)
                override {
            data_.set_cell(row, column, text);
        }

      private:
        T data_;
    };

    std::unique_ptr<Concept> self_;
};

class CellConstSpanPresenter final {
  public:
    explicit CellConstSpanPresenter(CellConstSpanView view);

    void set(asw::CellConstSpan const& cells);

  private:
    CellConstSpanView view_;
};

}  // namespace aswui