#include "Cell.h"
#include <random>

namespace asw {

namespace {

class RandomGenerator final : public PositionGenerator {
  public:
    ~RandomGenerator() override = default;

    [[nodiscard]] Position
    operator()(std::size_t const rows, std::size_t const columns) override {
        return {std::uniform_int_distribution<std::size_t>{0, rows - 1}(
                        generator),
                std::uniform_int_distribution<std::size_t>{0, columns - 1}(
                        generator)};
    }

  private:
    std::mt19937 generator = std::mt19937{std::random_device{}()};
};

}  // namespace

Vector2d<MineCell> generate_mines(
        std::size_t const rows,
        std::size_t const columns,
        int const count,
        PositionGenerator&& generator) {
    Vector2d<MineCell> mines{rows, columns};
    for (int i = 0; i < count; ++i) {
        auto const [row, column] = generator(rows, columns);
        mines(row, column) = MineCell::Mined;
    }
    return mines;
}

Vector2d<MineCell> generate_random_mines(
        std::size_t const rows,
        std::size_t const columns,
        int const count) {
    return generate_mines(rows, columns, count, RandomGenerator{});
}

}  // namespace asw