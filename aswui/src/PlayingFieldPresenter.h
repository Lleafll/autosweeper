#pragma once

namespace aswui {

class PlayingFieldView {
public:
    virtual ~PlayingFieldView() = default;
};

class PlayingFieldPresenter final {
public:
    explicit PlayingFieldPresenter(PlayingFieldView& view);

private:
    PlayingFieldView& view_;
};

}  // namespace aswui