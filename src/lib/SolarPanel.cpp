#include "SolarPanel.hpp"

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>

namespace tdr
{

auto powerOutput(SolarPanel const& panel, SolarPanel::Location const& location) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::area> auto area     = panel.width * panel.height;
    QuantityOf<isq::power> auto kWp     = area * (1.0 * kW / m2) * panel.efficiency;
    QuantityOf<isq::power> auto output  = area * location.irradiance * panel.efficiency;
    QuantityOf<isq::energy> auto energy = output * location.daylight;

    fmt::println("Solar panel:");
    fmt::println("-----------");
    fmt::println("Width:      {::N[.3f]}", panel.width.in(cm));
    fmt::println("Height:     {::N[.3f]}", panel.height.in(cm));
    fmt::println("Area:       {::N[.3f]}", area.in(m2));
    fmt::println("Efficiency: {}", panel.efficiency);
    fmt::println("kWp:        {}\n", kWp.in(kW));

    fmt::println("Irradiance: {}", location.irradiance);
    fmt::println("Daylight:   {}", location.daylight.in(h));
    fmt::println("Output:     {::N[.3f]}", output.in(kW));
    fmt::println("Energy:     {::N[.3f]}", energy.in(kW * h));
    fmt::println("");
}

}  // namespace tdr
