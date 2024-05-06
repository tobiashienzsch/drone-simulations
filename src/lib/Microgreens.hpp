#pragma once

#include "Finance.hpp"

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/math.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

struct GrowLight
{
    quantity<isq::power[si::watt]> power;
    quantity<isq::maximum_efficiency[percent]> efficiency;

    auto waste() const -> QuantityOf<isq::power> auto { return power * (1.0 * one - efficiency); }
    auto heat(QuantityOf<isq::volume> auto v) const -> QuantityOf<isq::thermodynamic_temperature / isq::time> auto
    {
        using namespace mp_units::si::unit_symbols;

        constexpr QuantityOf<isq::specific_heat_capacity> auto c_p = 1'005.0 * J / (kg * K);
        constexpr QuantityOf<isq::density> auto rho                = 1.225 * kg / m3;

        return waste().in(J / s) / (v * rho * c_p);
    }
};

struct IntermodalContainer
{
    quantity<isq::length[si::metre]> length;
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;
};

struct GrowRack
{
    quantity<isq::depth[si::metre]> depth;
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;

    quantity<one> shelfs;
    quantity<isq::width[si::metre]> tray;
};

[[nodiscard]] constexpr auto airConditionPower(QuantityOf<isq::volume> auto V,
                                               QuantityOf<isq::thermodynamic_temperature> auto delta_T,
                                               QuantityOf<isq::time> auto lightTime) -> QuantityOf<isq::power> auto
{
    using namespace mp_units::si::unit_symbols;

    constexpr QuantityOf<isq::specific_heat_capacity> auto c_p = 1'005.0 * J / (kg * K);
    constexpr QuantityOf<isq::density> auto rho                = 1.225 * kg / m3;

    auto const Q = c_p * rho * V * delta_T;
    return Q / lightTime;
}

auto growContainer(IntermodalContainer const& ic, GrowRack const& rack, GrowLight const& light) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::area> auto area     = ic.length * ic.width;
    QuantityOf<isq::volume> auto volume = area * ic.height;

    QuantityOf<dimensionless> auto rows   = 2 * one;
    QuantityOf<dimensionless> auto racks  = floor<one>(ic.length / rack.width) * rows;
    QuantityOf<dimensionless> auto shelfs = rack.shelfs * racks;
    QuantityOf<dimensionless> auto trays  = floor<one>(rack.width / rack.tray) * shelfs;

    QuantityOf<dimensionless> auto lightsPerShelf                    = 2.0 * one;
    QuantityOf<dimensionless> auto lights                            = lightsPerShelf * shelfs;
    QuantityOf<isq::power> auto lightPower                           = light.power * lights;
    QuantityOf<isq::power> auto waste                                = light.waste() * lights;
    QuantityOf<isq::thermodynamic_temperature / isq::time> auto heat = light.heat(volume) * lights;
    QuantityOf<isq::time> auto lightTime                             = (1.0 * h).in(s);
    QuantityOf<isq::thermodynamic_temperature> auto delta_T          = heat * lightTime;
    QuantityOf<isq::power> auto cooling      = tdr::airConditionPower(volume, delta_T, lightTime);
    QuantityOf<isq::power> auto totalPower   = lightPower + cooling;
    QuantityOf<isq::energy> auto totalEnergy = totalPower * 12 * h;

    fmt::println("GrowContainer:");
    fmt::println("-------------");
    fmt::println("Length:       {}", ic.length.in(m));
    fmt::println("Width:        {}", ic.width.in(m));
    fmt::println("Height:       {}", ic.height.in(m));
    fmt::println("Area:         {::N[.2f]}", area.in(m2));
    fmt::println("Volume:       {::N[.2f]}\n", volume.in(m3));

    fmt::println("Racks:        {}", racks);
    fmt::println("Shelfs:       {}", shelfs);
    fmt::println("Trays:        {}", trays);
    fmt::println("Tray-Area:    {}\n", (rack.tray * rack.depth * trays).in(m2));

    fmt::println("Light:        {}", light.power.in(W));
    fmt::println("Efficiency:   {}\n", light.efficiency.in(percent));

    fmt::println("Lights:       {}", lights);
    fmt::println("Lights-Power: {}", lightPower.in(W));
    fmt::println("Waste:        {::N[.2f]}", waste.in(W));
    fmt::println("Heat:         {::N[.5f]}", heat);
    fmt::println("Heat-1h:      {::N[.3f]}", delta_T);
    fmt::println("Cooling-1h:   {}", cooling.in(W));
    fmt::println("Power:        {}", totalPower.in(W));
    fmt::println("Energy:       {}", totalEnergy.in(kW * h));
    fmt::println("");
}

struct Microgreen
{
    quantity<isq::mass[si::gram]> seeds;
    quantity<finance::euro / si::kilo<si::gram>> price;

    quantity<one> light;
    quantity<isq::time[si::day]> growPhase;
    quantity<isq::time[si::day]> restPhase;

    quantity<isq::mass[si::gram]> harvist;
    quantity<finance::euro / si::kilo<si::gram>> msrp;
};

inline auto harvistReport(Microgreen const& plant) -> void
{
    using namespace mp_units::si::unit_symbols;
    using namespace finance::unit_symbols;

    QuantityOf<dimensionless> auto light = plant.light;
    QuantityOf<isq::time> auto growPhase = plant.growPhase;
    QuantityOf<isq::time> auto restPhase = plant.restPhase;
    QuantityOf<isq::mass> auto harvist   = plant.harvist;

    QuantityOf<finance::currency / isq::mass> auto msrp = plant.msrp;
    QuantityOf<finance::currency> auto price            = plant.price * plant.seeds;
    QuantityOf<finance::currency> auto value            = msrp * harvist;

    fmt::println("Microgreens:");
    fmt::println("-----------");
    fmt::println("Seeds:   {}", plant.seeds.in(g));
    fmt::println("Price:   {::N[.2f]}\n", price.in(EUR));

    fmt::println("Light:   {}", light.in(h / d));
    fmt::println("Grow:    {}", growPhase.in(d));
    fmt::println("Rest:    {}", restPhase.in(d));
    fmt::println("Cycle:   {}\n", (growPhase + restPhase).in(d));

    fmt::println("Harvist: {}", harvist.in(g));
    fmt::println("MSRP:    {}", msrp.in(EUR / kg));
    fmt::println("Value:   {::N[.2f]}", value.in(EUR));
    fmt::println("Profit:  {::N[.2f]}", (value - price).in(EUR));
    fmt::println("");
}

}  // namespace tdr
