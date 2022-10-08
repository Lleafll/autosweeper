#include "PlayingFieldWidgetQt.h"
#include "PlayingFieldPresenter.h"

namespace aswui {

class PlayingFieldWidgetQt::Impl final : public PlayingFieldView {
public:
    Impl() : presenter_{*this} {}

private:
    PlayingFieldPresenter presenter_;
};

PlayingFieldWidgetQt::PlayingFieldWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>()} {}

PlayingFieldWidgetQt::~PlayingFieldWidgetQt() = default;

}  // namespace aswui