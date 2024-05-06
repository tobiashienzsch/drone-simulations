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

        static constexpr QuantityOf<isq::specific_heat_capacity> auto c_p = 1'005.0 * J / (kg * K);
        static constexpr QuantityOf<isq::density> auto rho                = 1.225 * kg / m3;

        return waste().in(J / s) / (v * rho * c_p);
    }
};

[[nodiscard]] constexpr auto airConditionPower(QuantityOf<isq::volume> auto V,
                                               QuantityOf<isq::thermodynamic_temperature> auto delta_T,
                                               QuantityOf<isq::time> auto lightTime) -> QuantityOf<isq::power> auto
{
    using namespace mp_units::si::unit_symbols;

    static constexpr QuantityOf<isq::specific_heat_capacity> auto c_p = 1'005.0 * J / (kg * K);
    static constexpr QuantityOf<isq::density> auto rho                = 1.225 * kg / m3;

    auto const Q = c_p * rho * V * delta_T;
    return Q / lightTime;
}

struct IntermodalContainer
{
    quantity<isq::length[si::metre]> length;
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;

    [[nodiscard]] constexpr auto area() const noexcept -> QuantityOf<isq::area> auto { return length * width; }
    [[nodiscard]] constexpr auto volume() const noexcept -> QuantityOf<isq::volume> auto { return area() * height; }
};

struct GrowRack
{
    quantity<isq::depth[si::metre]> depth;
    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;

    quantity<one, int> shelfs;
    quantity<isq::width[si::metre]> tray;
};

struct GrowContainer
{
    IntermodalContainer container;
    GrowRack rack;
    GrowLight light;
    quantity<one, int> rows;
    quantity<one, int> lightsPerShelf;

    [[nodiscard]] constexpr auto racks() const noexcept -> QuantityOf<dimensionless> auto
    {
        return floor<one>(container.length / rack.width) * rows;
    }

    [[nodiscard]] constexpr auto shelfs() const noexcept -> QuantityOf<dimensionless> auto
    {
        return rack.shelfs * racks();
    }

    [[nodiscard]] constexpr auto trays() const noexcept -> QuantityOf<dimensionless> auto
    {
        return floor<one>(rack.width / rack.tray) * shelfs();
    }

    [[nodiscard]] constexpr auto trayArea() const noexcept -> QuantityOf<isq::area> auto
    {
        return rack.tray * rack.depth * trays();
    }

    [[nodiscard]] constexpr auto lights() const noexcept -> QuantityOf<dimensionless> auto
    {
        return lightsPerShelf * shelfs();
    }

    [[nodiscard]] constexpr auto powerLights() const noexcept -> QuantityOf<isq::power> auto
    {
        return light.power * lights();
    }

    [[nodiscard]] constexpr auto powerWaste() const noexcept -> QuantityOf<isq::power> auto
    {
        return light.waste() * lights();
    }

    [[nodiscard]] constexpr auto heat() const noexcept -> QuantityOf<isq::thermodynamic_temperature / isq::time> auto
    {
        return light.heat(container.volume()) * lights();
    }
};

auto report(GrowContainer const& gc) -> void
{
    using namespace mp_units::si::unit_symbols;
    using namespace finance::unit_symbols;

    QuantityOf<isq::time> auto lightTime                    = (1.0 * h).in(s);
    QuantityOf<isq::thermodynamic_temperature> auto delta_T = gc.heat() * lightTime;
    QuantityOf<isq::power> auto cooling    = tdr::airConditionPower(gc.container.volume(), delta_T, lightTime);
    QuantityOf<isq::power> auto totalPower = gc.powerLights() + cooling;
    QuantityOf<isq::energy / isq::time> auto totalEnergy = totalPower * 12 * h / d;

    auto energyCost = 0.31 * EUR / (kW * h);

    fmt::println("GrowContainer:");
    fmt::println("-------------");
    fmt::println("Length:       {}", gc.container.length.in(m));
    fmt::println("Width:        {}", gc.container.width.in(m));
    fmt::println("Height:       {}", gc.container.height.in(m));
    fmt::println("Area:         {::N[.2f]}", gc.container.area().in(m2));
    fmt::println("Volume:       {::N[.2f]}\n", gc.container.volume().in(m3));

    fmt::println("Racks:        {}", gc.racks());
    fmt::println("Shelfs:       {}", gc.shelfs());
    fmt::println("Trays:        {}", gc.trays());
    fmt::println("Tray-Area:    {}\n", gc.trayArea().in(m2));

    fmt::println("Light:        {}", gc.light.power.in(W));
    fmt::println("Efficiency:   {}\n", gc.light.efficiency.in(percent));

    fmt::println("Lights:       {}", gc.lights());
    fmt::println("Lights-Power: {}", gc.powerLights().in(W));
    fmt::println("Waste:        {::N[.2f]}", gc.powerWaste().in(W));
    fmt::println("Heat:         {::N[.5f]}", gc.heat());
    fmt::println("Heat-1h:      {::N[.3f]}", delta_T);
    fmt::println("Cooling-1h:   {}", cooling.in(W));
    fmt::println("Power:        {}", totalPower.in(W));
    fmt::println("Energy:       {}", totalEnergy.in(kW * h / d));
    fmt::println("Energy-Cost:  {::N[.2f]}", (energyCost * totalEnergy).in(EUR / d));
    fmt::println("");
}

struct Microgreen
{
    quantity<finance::euro / si::kilo<si::gram>> price;

    quantity<isq::mass[si::gram]> seeds;
    quantity<isq::volume[si::litre] / isq::time[si::day]> water;
    quantity<isq::time[si::hour] / isq::time[si::day]> light;

    quantity<isq::time[si::day]> germination;
    quantity<isq::time[si::day]> grow;
    quantity<isq::time[si::day]> rest;

    quantity<isq::mass[si::gram]> harvist;
    quantity<finance::euro / si::kilo<si::gram>> msrp;
};

inline auto report(Microgreen const& plant) -> void
{
    using namespace mp_units::si::unit_symbols;
    using namespace finance::unit_symbols;

    QuantityOf<finance::currency> auto price = plant.price * plant.seeds;
    QuantityOf<finance::currency> auto value = plant.msrp * plant.harvist;
    QuantityOf<isq::time> auto cycle         = plant.germination + plant.grow + plant.rest;

    fmt::println("Microgreens:");
    fmt::println("-----------");
    fmt::println("Seeds:       {}", plant.seeds.in(g));
    fmt::println("Price:       {::N[.2f]}\n", price.in(EUR));

    fmt::println("Water:       {}", plant.water.in(si::milli<si::litre> / d));
    fmt::println("Light:       {}", plant.light.in(h / d));
    fmt::println("Germination: {}", plant.germination.in(d));
    fmt::println("Grow:        {}", plant.grow.in(d));
    fmt::println("Rest:        {}", plant.rest.in(d));
    fmt::println("Cycle:       {}\n", cycle.in(d));

    fmt::println("Water-Usage: {}", (plant.water * (plant.grow + plant.rest)).in(l));
    fmt::println("Harvist:     {}", plant.harvist.in(g));
    fmt::println("MSRP:        {}", plant.msrp.in(EUR / kg));
    fmt::println("Value:       {::N[.2f]}", value.in(EUR));
    fmt::println("Profit:      {::N[.2f]}", (value - price).in(EUR));
    fmt::println("");
}

}  // namespace tdr
