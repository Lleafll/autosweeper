#pragma once

namespace asw {
class PlayingField;
}  // namespace asw

namespace asw {

class Solver final {
public:
    enum class Result { Success, Lost };

    explicit Solver(PlayingField& field);

    Result next();

private:
    PlayingField& field_;
};

}  // namespace asw