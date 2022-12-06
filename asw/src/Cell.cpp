#include "Cell.h"
#include <random>

namespace asw {

namespace {

class RandomGenerator final : public PositionGenerator {
  public:
    ~RandomGenerator() override = default;

    [[nodiscard]] Position operator()(Size const& size) override {
        return {std::uniform_int_distribution<size_t>{0, size.rows - 1}(
                        generator),
                std::uniform_int_distribution<size_t>{0, size.columns - 1}(
                        generator)};
    }

  private:
    std::mt19937 generator = std::mt19937{std::random_device{}()};
};

}  // namespace

Vector2d<MineCell> generate_mines(
        Size const& size,
        int const count,
        PositionGenerator&& generator) {
    Vector2d<MineCell> mines{size};
    for (int i = 0; i < count; ++i) {
        auto const [row, column] = generator(size);
        mines(row, column) = MineCell::Mined;
    }
    return mines;
}

Vector2d<MineCell> generate_random_mines(Size const& size, int const count) {
    return generate_mines(size, count, RandomGenerator{});
}

}  // namespace asw