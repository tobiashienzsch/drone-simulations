#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/math.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

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

    static constexpr auto T_0 = 288.15 * K;      // sea-level standard temperature
    static constexpr auto L   = 0.0065 * K / m;  // standard temperature lapse rate in the troposphere

    return T_0 - L * altitude;
}

[[nodiscard]] constexpr auto pressureAt(QuantityOf<isq::altitude> auto altitude) -> QuantityOf<isq::pressure> auto
{
    using namespace mp_units::si::unit_symbols;

    static constexpr auto P_0 = 101325.0 * Pa;         // sea-level standard atmospheric pressure
    static constexpr auto T_0 = 288.15 * K;            // sea-level standard temperature
    static constexpr auto L   = 0.0065 * K / m;        // standard temperature lapse rate in the troposphere
    static constexpr auto M   = 0.0289644 * kg / mol;  // molar mass of Earth's air
    static constexpr auto R   = (1.0 * universal_gas_constant).in(J / (mol * K));  // universal gas constant

    auto const base = T_0 / (T_0 + L * altitude.in(si::metre));
    auto const exp  = (1.0 * si::standard_gravity * M) / (R * L);

    auto const P = P_0 * std::pow(base.numerical_value_ref_in(base.unit), exp.numerical_value_ref_in(exp.unit));
    return P;
}

}  // namespace tdr
