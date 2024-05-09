#pragma once

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tdr
{

using namespace mp_units;

struct IntermodalContainer
{
    quantity<isq::length[si::metre]> length;
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;

    [[nodiscard]] constexpr auto area() const noexcept -> QuantityOf<isq::area> auto { return length * width; }
    [[nodiscard]] constexpr auto volume() const noexcept -> QuantityOf<isq::volume> auto { return area() * height; }
};

}  // namespace tdr
