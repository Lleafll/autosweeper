#pragma once

namespace aswui {

class MinePredictionsView {
  public:
    virtual ~MinePredictionsView() = default;

  private:
    friend class MinePredictionsPresenter;
};

class MinePredictionsPresenter final {
  public:
    explicit MinePredictionsPresenter(MinePredictionsView& view);

  private:
    MinePredictionsView& view_;
};

}  // namespace aswui