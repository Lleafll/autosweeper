#pragma once

#include "Cell.h"
#include "PlayingField.h"
#include <experimental/mdspan>
#include <vector>

namespace asw {

class InMemoryPlayingField final : public PlayingField {
  public:
    explicit InMemoryPlayingField(ConstMineCellSpan const& mines);

    ~InMemoryPlayingField() override = default;

    [[nodiscard]] std::size_t rows() const override;
    [[nodiscard]] std::size_t columns() const override;
    [[nodiscard]] int mine_count() const override;
    [[nodiscard]] Cell
    operator()(std::size_t row, std::size_t column) const override;
    void reveal(std::size_t row, std::size_t column) override;
    [[nodiscard]] CellSpan span() override;
    [[nodiscard]] ConstCellSpan cspan() const override;

    bool operator==(InMemoryPlayingField const&) const = default;

  private:
    std::size_t rows_;
    std::size_t columns_;
    std::vector<Cell> hidden_;
    std::vector<Cell> cells_;

    [[nodiscard]] ConstCellSpan hidden() const;
    [[nodiscard]] ConstCellSpan cells() const;
};

}  // namespace asw