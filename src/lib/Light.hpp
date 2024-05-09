#pragma once

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tug
{

using namespace mp_units;

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

}  // namespace tug
