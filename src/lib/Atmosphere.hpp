#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/math.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tug
{

using namespace mp_units;

inline constexpr struct bar : named_unit<"bar", mag_ratio<100'000, 1> * si::pascal>
{
} bar;

inline constexpr struct standard_atmosphere : named_unit<"atm", mag_ratio<101'325, 1> * si::pascal>
{
} standard_atmosphere;

inline constexpr auto atm = standard_atmosphere;

inline constexpr auto universal_gas_constant = si::si2019::boltzmann_constant * si::si2019::avogadro_constant;

[[nodiscard]] constexpr auto
temperatureAt(QuantityOf<isq::altitude> auto altitude) -> QuantityOf<isq::thermodynamic_temperature> auto
{
    using namespace mp_units::si::unit_symbols;

    constexpr auto T_0 = 288.15 * K;      // sea-level standard temperature
    constexpr auto L   = 0.0065 * K / m;  // standard temperature lapse rate in the troposphere

    return T_0 - L * altitude;
}

[[nodiscard]] constexpr auto pressureAt(QuantityOf<isq::altitude> auto altitude) -> QuantityOf<isq::pressure> auto
{
    using namespace mp_units::si::unit_symbols;

    constexpr auto P_0 = 101325.0 * Pa;                                     // sea-level standard atmospheric pressure
    constexpr auto T_0 = 288.15 * K;                                        // sea-level standard temperature
    constexpr auto M   = 0.0289644 * kg / mol;                              // molar mass of Earth's air
    constexpr auto R_0 = (1.0 * universal_gas_constant).in(J / (mol * K));  // universal gas constant
    constexpr auto g   = (1.0 * si::standard_gravity).in(m / s2);           // gravity

    return P_0 * exp(-(g * altitude.in(m)*M) / (T_0 * R_0));
}

[[nodiscard]] constexpr auto densityAt(QuantityOf<isq::altitude> auto altitude) -> QuantityOf<isq::density> auto
{
    using namespace mp_units::si::unit_symbols;

    constexpr auto R = 1.0 * universal_gas_constant;  // universal gas constant
    constexpr auto M = 0.0289644 * kg / mol;          // molar mass of Earth's air

    QuantityOf<isq::pressure> auto const P                  = pressureAt(altitude);
    QuantityOf<isq::thermodynamic_temperature> auto const T = temperatureAt(altitude);

    return (P * M) / (R * T);
}

}  // namespace tug
