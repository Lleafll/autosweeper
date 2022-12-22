#pragma once

#include "IScreen.h"
#include "Image.h"
#include "Match.h"
#include "PlayingField.h"
#include <di/ptr.h>
#include <optional>

namespace asw {

class ImageMatchingPlayingField final : public PlayingField {
  public:
    using Matcher = std::function<std::vector<Match>(ImageConstSpan const&)>;

    /**
     * @brief Constructs a field for Minesweeper Classic
     */
    ImageMatchingPlayingField();

    ImageMatchingPlayingField(
            di::ptr<IScreen> screen,
            Matcher matcher,
            int distance_between_cells);

    ~ImageMatchingPlayingField() override = default;

    [[nodiscard]] size_t rows() const override;
    [[nodiscard]] size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    Cell operator()(size_t row, size_t column) const override;
    void reveal(Position const& position) override;
    [[nodiscard]] CellConstSpan cspan() const override;

  private:
    di::ptr<IScreen> screen_;
    Matcher matcher_;
    Vector2d<Cell> field_ = Vector2d<Cell>{{0, 0}, Cell::Hidden};
};

}  // namespace asw