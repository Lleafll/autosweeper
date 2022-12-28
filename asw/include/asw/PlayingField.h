#pragma once

#include "Cell.h"
#include "Position.h"
#include "Size.h"
#include <proxy/proxy.h>

namespace asw {

struct GetSize final : pro::dispatch<Size()> {
    Size operator()(auto const& self) const {
        return {self.rows(), self.columns()};
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

struct PlayingField final : pro::facade<GetSize, Reveal, Cspan, Update> {};

}  // namespace asw
