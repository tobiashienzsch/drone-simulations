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

        constexpr QuantityOf<isq::specific_heat_capacity> auto capacity = 1'005.0 * J / (kg * K);
        constexpr QuantityOf<isq::density> auto density                 = 1.225 * kg / m3;

        return waste().in(J / s) / (v * density * capacity);
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

auto growContainer(IntermodalContainer const& ic, GrowRack const& rack, GrowLight const& light) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::area> auto area     = ic.length * ic.width;
    QuantityOf<isq::volume> auto volume = area * ic.height;

    QuantityOf<dimensionless> auto rows   = 2 * one;
    QuantityOf<dimensionless> auto racks  = floor<one>(ic.length / rack.width) * rows;
    QuantityOf<dimensionless> auto shelfs = rack.shelfs * racks;
    QuantityOf<dimensionless> auto trays  = floor<one>(rack.width / rack.tray) * shelfs;

    QuantityOf<dimensionless> auto lightsPerShelf = 2.0 * one;
    QuantityOf<dimensionless> auto lights         = lightsPerShelf * shelfs;
    QuantityOf<isq::power> auto power             = light.power * lights;
    QuantityOf<isq::power> auto waste             = light.waste() * lights;
    auto heat                                     = light.heat(volume) * lights;

    fmt::println("GrowContainer:");
    fmt::println("-------------");
    fmt::println("Length:     {}", ic.length.in(m));
    fmt::println("Width:      {}", ic.width.in(m));
    fmt::println("Height:     {}", ic.height.in(m));
    fmt::println("Area:       {::N[.2f]}", area.in(m2));
    fmt::println("Volume:     {::N[.2f]}\n", volume.in(m3));
    fmt::println("Racks:      {}", racks);
    fmt::println("Shelfs:     {}", shelfs);
    fmt::println("Trays:      {}", trays);
    fmt::println("Tray-Area:  {}\n", (rack.tray * rack.depth * trays).in(m2));
    fmt::println("Light:      {}", light.power.in(W));
    fmt::println("Efficiency: {}\n", light.efficiency.in(percent));
    fmt::println("Lights:     {}", lights);
    fmt::println("Power:      {}", power.in(W));
    fmt::println("Waste:      {::N[.2f]}", waste.in(W));
    fmt::println("Heat:       {}", heat);
    fmt::println("Heat-12h:   {::N[.3f]}", heat * 1800.0 * s);
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
