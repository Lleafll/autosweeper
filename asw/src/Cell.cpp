#include "Cell.h"
#include <random>

namespace asw {

namespace {

class RandomGenerator {
  public:
    [[nodiscard]] Position operator()(Size const& size) {
        return {std::uniform_int_distribution<size_t>{0, size.rows - 1}(
                        generator),
                std::uniform_int_distribution<size_t>{0, size.columns - 1}(
                        generator)};
    }

  private:
    std::mt19937 generator = std::mt19937{std::random_device{}()};
};

}  // namespace

FixedArray2d<MineCell> generate_random_mines(Size const& size, int const count) {
    return generate_mines(size, count, RandomGenerator{});
}

}  // namespace asw