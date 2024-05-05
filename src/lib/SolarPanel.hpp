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
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;
    quantity<isq::maximum_efficiency[percent]> efficiency;
};

struct SolarPanelLocation
{
    quantity<isq::irradiance[si::watt / square(si::metre)]> irradiance;
    quantity<isq::time[si::hour]> daylight;
};

inline auto powerOutput(SolarPanel const& panel, SolarPanelLocation const& location) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::area> auto area     = panel.width * panel.height;
    QuantityOf<isq::power> auto power   = area * location.irradiance * panel.efficiency;
    QuantityOf<isq::time> auto duration = location.daylight;
    QuantityOf<isq::energy> auto energy = power * duration;

    fmt::println("Solar panel:");
    fmt::println("------------");
    fmt::println("Width:      {::N[.3f]}", panel.width.in(cm));
    fmt::println("Height:     {::N[.3f]}", panel.height.in(cm));
    fmt::println("Area:       {::N[.3f]}", area.in(m2));
    fmt::println("Irradiance: {}", location.irradiance);
    fmt::println("Efficiency: {}", panel.efficiency);
    fmt::println("Power:      {::N[.3f]}", power.in(W));
    fmt::println("Energy:     {::N[.3f]}", energy.in(W * h));
    fmt::println("");
}

}  // namespace tdr
