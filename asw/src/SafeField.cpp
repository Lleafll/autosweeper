#include "SafeField.h"
#include "PlayingFieldWithMines.h"
#include <experimental/mdspan>

namespace stdex = std::experimental;

namespace asw {

SafeField::SafeField(std::size_t const rows, std::size_t const columns)
    : rows_{rows},
      columns_{columns},
      buffer_(rows * columns, Cell::Other) {}

SafeField::View SafeField::view() {
    return View{buffer_.data(), rows_, columns_};
}

SafeField::ConstView SafeField::view() const {
    return ConstView{buffer_.data(), rows_, columns_};
}

namespace {

struct UnsafeCell final {
    std::size_t row;
    std::size_t column;

    bool operator==(UnsafeCell const&) const = default;
};

struct UnsafeSet final {
    std::vector<UnsafeCell> cells;

    [[nodiscard]] bool is_subset_of(UnsafeSet const& other) const {
        if (cells.size() > other.cells.size()) {
            return false;
        }
        // O(N²) :(
        return std::ranges::all_of(
                other.cells, [this](UnsafeCell const& other_cell) -> bool {
                    return std::ranges::find(cells, other_cell) != cells.cend();
                });
    }

    bool operator==(UnsafeSet const&) const = default;
};

struct UnsafeSets final {
    std::vector<UnsafeSet> sets;

    void add(UnsafeSet&& new_set) {
        for (auto& set: sets) {
            if (new_set.is_subset_of(set)) {
                return;
            }
            if (set.is_subset_of(new_set)) {
                set = std::move(new_set);
                return;
            }
        }
    }
};

UnsafeSet get_surrounding_unsafe_set(
        PlayingFieldWithMines::ConstView const& view,
        std::size_t const row,
        std::size_t const column) {
    UnsafeSet set;

    return set;
}

UnsafeSets get_unsafe_cells(PlayingFieldWithMines const& field) {
    auto const view = field.view();
    auto const rows = view.extent(0);
    auto const columns = view.extent(1);
    UnsafeSets unsafe_cells;
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t column = 0; column < columns; ++column) {
            auto const cell = view(row, column);
            if (cell != PlayingField::Cell::Hidden) {
                continue;
            }
            unsafe_cells.add(get_surrounding_unsafe_set(view, row, column));
        }
    }
    return unsafe_cells;
}

}  // namespace

SafeField find_safe_cells(PlayingFieldWithMines const& field) {
    auto const mines_view = field.view();
    auto const rows = mines_view.extent(0);
    auto const columns = mines_view.extent(1);
    auto const unsafe_cells = get_unsafe_cells(field);
    SafeField safe_field{rows, columns};
    return safe_field;
}

}  // namespace asw
