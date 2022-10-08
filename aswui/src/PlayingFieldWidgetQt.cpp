#include "PlayingFieldWidgetQt.h"
#include "PlayingFieldPresenter.h"

namespace aswui {

class PlayingFieldWidgetQt::Impl final : public PlayingFieldView {
public:
    Impl() : presenter_{*this} {
    }

    ~Impl() override = default;

private:
    void set_row_count(std::size_t rows) override {
        // TODO: Implement
    }

    void set_column_count(std::size_t columns) override {
        // TODO: Implement
    }

private:
    PlayingFieldPresenter presenter_;
};

PlayingFieldWidgetQt::PlayingFieldWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>()} {
}

PlayingFieldWidgetQt::~PlayingFieldWidgetQt() = default;

}  // namespace aswui