#pragma once

#include "Cell.h"
#include "Position.h"
#include <proxy/proxy.h>

namespace asw {

struct Rows final : pro::dispatch<size_t()> {
    size_t operator()(auto const& self) const {
        return self.rows();
    }
};

struct Columns final : pro::dispatch<size_t()> {
    size_t operator()(auto const& self) const {
        return self.columns();
    }
};

struct MineCount final : pro::dispatch<int()> {
    int operator()(auto const& self) const {
        return self.mine_count();
    }
};

struct GetCell final : pro::dispatch<Cell(size_t, size_t)> {
    Cell
    operator()(auto const& self, size_t const row, size_t const column) const {
        return self(row, column);
    }
};

struct Reveal final : pro::dispatch<void(Position const&)> {
    void operator()(auto& self, Position const& position) {
        self.reveal(position);
    }
};

struct Cspan final : pro::dispatch<CellConstSpan()> {
    CellConstSpan operator()(auto const& self) const {
        return self.cspan();
    }
};

struct Update final : pro::dispatch<void()> {
    void operator()(auto& self) {
        self.update();
    }
};

struct PlayingField final
    : pro::facade<Rows, Columns, MineCount, GetCell, Reveal, Cspan, Update> {};

}  // namespace asw
