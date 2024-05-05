#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

struct SolarPanel
{
    quantity<square(si::metre)> area;
    quantity<si::watt / square(si::metre)> irradiance;
    quantity<one> efficiency;
};

inline auto powerOutput(SolarPanel panel) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::power> auto power   = panel.area * panel.irradiance * panel.efficiency;
    QuantityOf<isq::time> auto duration = 5.0 * h;
    QuantityOf<isq::energy> auto energy = power * duration;

    fmt::println("Solar panel:");
    fmt::println("------------");
    fmt::println("Area:       {}", panel.area.in(m2));
    fmt::println("Irradiance: {}", panel.irradiance);
    fmt::println("Efficiency: {}", panel.efficiency);
    fmt::println("Power:      {}", power.in(W));
    fmt::println("Energy:     {}", energy.in(W * h));
    fmt::println("");
}

}  // namespace tdr
