#pragma once

#include "Finance.hpp"
#include "IntermodalContainer.hpp"
#include "Light.hpp"

#include <mp-units/math.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include <filesystem>
#include <vector>

namespace tug
{

using namespace mp_units;

struct Microgreen
{
    std::string name;
    quantity<finance::euro / si::kilogram> price;

    quantity<isq::mass[si::gram] / isq::area[square(si::metre)]> seeds;
    quantity<isq::volume[si::litre] / isq::time[si::day]> water;
    quantity<isq::time[si::hour] / isq::time[si::day]> light;

    quantity<isq::time[si::day]> germination;
    quantity<isq::time[si::day]> grow;
    quantity<isq::time[si::day]> rest;

    quantity<isq::mass[si::gram]> yield;
    quantity<finance::euro / si::kilogram> msrp;
};

[[nodiscard]] auto loadMicrogreens(std::filesystem::path const& path) -> std::vector<Microgreen>;

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

auto report(GrowContainer const& gc) -> void;
auto report(GrowContainer const& gc, Microgreen const& plant) -> void;

}  // namespace tug
