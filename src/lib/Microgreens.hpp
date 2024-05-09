#pragma once

#include "Finance.hpp"

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/math.h>
#include <mp-units/systems/international.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
    std::string name;
    quantity<finance::euro / si::kilo<si::gram>> price;

    quantity<isq::mass[si::gram]> seeds;
    quantity<isq::volume[si::litre] / isq::time[si::day]> water;
    quantity<isq::time[si::hour] / isq::time[si::day]> light;

    quantity<isq::time[si::day]> germination;
    quantity<isq::time[si::day]> grow;
    quantity<isq::time[si::day]> rest;

    quantity<isq::mass[si::gram]> yield;
    quantity<finance::euro / si::kilo<si::gram>> msrp;
};

inline auto report(Microgreen const& plant, GrowContainer const& gc) -> void
{
    using namespace mp_units::si::unit_symbols;
    using namespace finance::unit_symbols;

    QuantityOf<finance::currency> auto price = plant.price * plant.seeds;
    QuantityOf<finance::currency> auto value = plant.msrp * plant.yield;
    QuantityOf<isq::time> auto cycle         = plant.germination + plant.grow + plant.rest;
    QuantityOf<dimensionless> auto cycles    = 30.0 * d / plant.grow;

    fmt::println("Microgreens-Tray(1020):");
    fmt::println("----------------------");
    fmt::println("Seeds:       {}", plant.seeds.in(g));
    fmt::println("Price:       {::N[.2f]}\n", price.in(EUR));

    fmt::println("Water:       {}", plant.water.in(si::milli<si::litre> / d));
    fmt::println("Light:       {}", plant.light.in(h / d));
    fmt::println("Germination: {}", plant.germination.in(d));
    fmt::println("Grow:        {}", plant.grow.in(d));
    fmt::println("Rest:        {}", plant.rest.in(d));
    fmt::println("Cycle:       {}", cycle.in(d));
    fmt::println("Cycles:      {::N[.2f]}\n", cycles);

    fmt::println("Water-Usage: {}", (plant.water * (plant.grow + plant.rest)).in(l));
    fmt::println("Yield:       {::N[.2f]}", plant.yield.in(g));
    fmt::println("MSRP:        {}", plant.msrp.in(EUR / kg));
    fmt::println("Value:       {::N[.2f]}", value.in(EUR));
    fmt::println("Profit:      {::N[.2f]}", (value - price).in(EUR));
    fmt::println("");

    // fmt::println("Microgreens-Container(Cycle):");
    // fmt::println("----------------------------");
    // fmt::println("Seeds:       {::N[.2f]}", (plant.seeds * gc.trays()).in(kg));
    // fmt::println("Price:       {::N[.2f]}\n", (price * gc.trays()).in(EUR));

    // fmt::println("Water-Usage: {::N[.2f]}", (plant.water * (plant.grow + plant.rest) * gc.trays()).in(l));
    // fmt::println("Yield:       {::N[.2f]}", (plant.yield * gc.trays()).in(kg));
    // fmt::println("Value:       {::N[.2f]}", (value * gc.trays()).in(EUR));
    // fmt::println("Profit:      {::N[.2f]}", (value * gc.trays() - price * gc.trays()).in(EUR));
    // fmt::println("");

    fmt::println("Microgreens-Container(Month):");
    fmt::println("----------------------------");
    fmt::println("Seeds:       {::N[.2f]}", (plant.seeds * gc.trays() * cycles).in(kg));
    fmt::println("Price:       {::N[.2f]}\n", (price * gc.trays() * cycles).in(EUR));

    fmt::println("Water-Usage: {::N[.2f]}", (plant.water * (plant.grow + plant.rest) * gc.trays() * cycles).in(l));
    fmt::println("Yield:       {::N[.2f]}", (plant.yield * gc.trays() * cycles).in(kg));
    fmt::println("Value:       {::N[.2f]}", (value * gc.trays() * cycles).in(EUR));
    fmt::println("Profit:      {::N[.2f]}", (value * gc.trays() * cycles - price * gc.trays() * cycles).in(EUR));
    fmt::println("");
}

inline auto loadMicrogreens(std::filesystem::path const& path) -> std::vector<Microgreen>
{
    auto file = std::ifstream{path};

    auto line = std::string{};

    // skip header
    std::getline(file, line);

    auto result = std::vector<Microgreen>{};
    while (std::getline(file, line))
    {
        auto ss    = std::stringstream(line);
        auto token = std::string{};

        // skip part number
        std::getline(ss, token, ',');

        // name
        auto name = std::string{};
        std::getline(ss, name, ',');

        // seeds / tray
        std::getline(ss, token, ',');
        auto const seedsPerTray = std::stod(token) * si::gram;

        // yield / tray
        std::getline(ss, token, ',');
        auto const yieldPerTray = std::stod(token) * international::ounce;

        // days / tray
        std::getline(ss, token, ',');
        auto const daysPerTray = std::stod(token) * si::day;

        // seed-price / 25lb
        std::getline(ss, token, ',');
        auto const seedPrice = std::stod(token) * finance::euro / (25.0 * international::pound);

        result.push_back(Microgreen{
            .name = name,

            .price = seedPrice,
            .seeds = seedsPerTray,
            .water = 0.25 * si::litre / si::day,
            .light = 8.0 * si::hour / si::day,

            .germination = 0.0 * si::day,
            .grow        = daysPerTray,
            .rest        = 2.0 * si::day,

            .yield = yieldPerTray,
            .msrp  = 13.0 * finance::euro / si::kilogram,
        });
    }

    return result;
}

}  // namespace tdr
