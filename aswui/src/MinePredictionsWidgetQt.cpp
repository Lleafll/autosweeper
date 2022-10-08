#include "MinePredictionsWidgetQt.h"
#include "MinePredictionsPresenter.h"

namespace aswui {

class MinePredictionsWidgetQt::Impl final : public MinePredictionsView {
  public:
    Impl() : presenter_{*this} {
    }

    ~Impl() override = default;

  private:
    MinePredictionsPresenter presenter_;
};

MinePredictionsWidgetQt::MinePredictionsWidgetQt(QWidget* const parent)
    : QWidget{parent},
      impl_{std::make_unique<Impl>()} {
}

MinePredictionsWidgetQt::~MinePredictionsWidgetQt() = default;

}  // namespace aswui