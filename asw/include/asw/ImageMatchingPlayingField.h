#pragma once

#include "Image.h"
#include "Match.h"
#include "MinesweeperScreen.h"
#include "PlayingField.h"
#include "find_in_image.h"
#include <optional>

namespace asw {

template<class T>
concept Screen = requires(T t) {
                     { t.grab() } -> std::convertible_to<std::optional<Image>>;
                     t.click(Position{});
                 };

template<class T>
concept MatcherFunc = requires(T t) {
                          {
                              t(ImageConstSpan{})
                          } -> std::convertible_to<std::vector<Match>>;
                      };

namespace detail {

std::tuple<FixedArray2d<Cell>, Position>
matches_to_field(std::span<Match const> matches, int distance_between_cells);

}  // namespace detail

template<Screen TScreen, MatcherFunc TMatcherFunc>
class ImageMatchingPlayingField {
  public:
    ImageMatchingPlayingField(
            TScreen screen,
            TMatcherFunc matcher,
            int const distance_between_cells)
        : screen_{std::move(screen)},
          matcher_{std::move(matcher)},
          distance_between_cells_{distance_between_cells} {
        update();
    }

    [[nodiscard]] size_t rows() const {
        return field_.cspan().extent(0);
    }

    [[nodiscard]] size_t columns() const {
        return field_.cspan().extent(1);
    }

    Cell operator()(size_t const row, size_t const column) const {
        return field_(row, column);
    }

    void reveal(Position const& position) {
        screen_.click(
                {bottom_left_.column +
                         position.column * distance_between_cells_,
                 bottom_left_.row + position.row * distance_between_cells_});
    }

    [[nodiscard]] CellConstSpan cspan() const {
        return field_.cspan();
    }

    void update() {
        auto const grab = screen_.grab();
        if (not grab.has_value()) {
            return;
        }
        auto const matches = matcher_(grab->cspan());
        if (matches.empty()) {
            return;
        }
        std::tie(field_, bottom_left_) =
                detail::matches_to_field(matches, distance_between_cells_);
    }

  private:
    TScreen screen_;
    TMatcherFunc matcher_;
    FixedArray2d<Cell> field_ = FixedArray2d<Cell>{{0, 0}, Cell::Hidden};
    int distance_between_cells_;
    Position bottom_left_ = {0, 0};
};

}  // namespace asw