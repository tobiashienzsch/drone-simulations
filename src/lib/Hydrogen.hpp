#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tug
{

using namespace mp_units;

[[nodiscard]] constexpr auto hydrogenEnergy(QuantityOf<isq::mass_density> auto density,
                                            QuantityOf<isq::volume> auto volume) -> QuantityOf<isq::energy> auto
{
    using namespace mp_units::si::unit_symbols;
    constexpr auto const lowerHeatingValue = 33.3 * kW * h / kg;
    return density * volume * lowerHeatingValue;
}

inline auto hydrogenEnergyIn(QuantityOf<isq::volume> auto volume) -> void
{
    using namespace mp_units::si::unit_symbols;

    static constexpr QuantityOf<isq::density> auto densityGas    = 0.08988 * kg / m3;
    static constexpr QuantityOf<isq::density> auto densityLiquid = 70.85 * kg / m3;

    QuantityOf<isq::energy> auto energyGas    = hydrogenEnergy(densityGas, volume);
    QuantityOf<isq::energy> auto energyLiquid = hydrogenEnergy(densityLiquid, volume);

    fmt::println("Hydrogen energy per volume:");
    fmt::println("--------------------------");
    fmt::println("Volume:         {::N[.3f]}", volume);
    fmt::println("Density Gas:    {::N[.3f]}", densityGas);
    fmt::println("Mass Gas:       {::N[.3f]}", (densityGas * volume).in(g));
    fmt::println("Energy Gas:     {::N[.3f]}\n", energyGas.in(kW * h));

    fmt::println("Density Liquid: {::N[.3f]}", densityLiquid);
    fmt::println("Mass Liquid:    {::N[.3f]}", (densityLiquid * volume).in(g));
    fmt::println("Energy Liquid:  {::N[.3f]}\n", energyLiquid.in(kW * h));

    fmt::println("Increase:       {::N[.3f]}x", energyLiquid / energyGas);
    fmt::println("");
}

inline auto compressGas() -> void
{
    using namespace mp_units::si::unit_symbols;

    // P = 200 * 100000  # Pressure in pascals (200 bar converted to Pascals)
    // V = 5  # Volume in liters
    // R = 8.314  # Universal gas constant in J/(mol·K)
    // T = 298  # Temperature in Kelvin (25 degrees Celsius)
    //
    // # Calculate moles of hydrogen
    // n = (P * V) / (R * T)

    auto R = (1.0 * si::si2019::boltzmann_constant * si::si2019::avogadro_constant).in(J / (mol * K));

    QuantityOf<isq::pressure> auto P                                    = 200.0 * 100'000.0 * Pa;
    QuantityOf<isq::volume> auto V                                      = 5.0 * l;
    QuantityOf<isq::thermodynamic_temperature> auto T                   = 298.0 * K;
    QuantityOf<isq::amount_of_substance> auto moles                     = (P * V) / (R * T);
    QuantityOf<isq::mass / isq::amount_of_substance> auto molecularMass = 2.0 * g / mol;
    QuantityOf<isq::mass> auto mass                                     = moles * molecularMass;

    fmt::println("Compress gas:");
    fmt::println("-------------");
    fmt::println("Pressure:     {}", P);
    fmt::println("Volume:       {}", V);
    fmt::println("Gas Constant: {}", R);
    fmt::println("Temperature:  {}", T);
    fmt::println("Moles:        {}", moles);
    fmt::println("Mass:         {}", mass);
    fmt::println("");
}

}  // namespace tug
