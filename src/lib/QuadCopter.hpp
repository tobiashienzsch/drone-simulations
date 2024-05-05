#pragma once

#include "Atmosphere.hpp"

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/math.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

struct QuadCopter
{
    quantity<isq::mass[si::kilo<si::gram>]> weight;
    quantity<isq::area[square(si::metre)]> frontalArea;
    quantity<isq::maximum_efficiency[percent]> thrustEfficiency;
    quantity<isq::maximum_efficiency[percent]> aerodynamicEfficiency;
};

struct Flight
{
    quantity<isq::distance[si::metre]> distance;
    quantity<si::metre> altitude;
    quantity<isq::speed[si::metre / si::second]> speed;

    [[nodiscard]] constexpr auto withDistance(QuantityOf<isq::distance> auto newDistance) const -> Flight
    {
        auto tmp     = *this;
        tmp.distance = newDistance;
        return tmp;
    }

    [[nodiscard]] constexpr auto withAltitude(QuantityOf<isq::altitude> auto newAltitude) const -> Flight
    {
        auto tmp     = *this;
        tmp.altitude = newAltitude;
        return tmp;
    }

    [[nodiscard]] constexpr auto withSpeed(QuantityOf<isq::speed> auto newSpeed) const -> Flight
    {
        auto tmp  = *this;
        tmp.speed = newSpeed;
        return tmp;
    }
};

inline auto estimatePowerConsumption(QuadCopter const& copter, Flight const& flight) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::altitude> auto altitude = flight.altitude;
    QuantityOf<isq::length> auto distance   = flight.distance;
    QuantityOf<isq::time> auto flightTime   = distance / flight.speed;

    QuantityOf<isq::mass> auto weight              = copter.weight;
    QuantityOf<isq::area> auto A_f                 = copter.frontalArea;
    QuantityOf<isq::speed> auto v_h                = flight.speed;
    QuantityOf<isq::speed> auto v_v                = 10.0 * m / s;
    QuantityOf<isq::density> auto rho              = densityAt(altitude);
    QuantityOf<isq::maximum_efficiency> auto eta_t = copter.thrustEfficiency;
    QuantityOf<isq::maximum_efficiency> auto eta_p = copter.aerodynamicEfficiency;
    QuantityOf<isq::drag_factor> auto C_D          = 60.0 * percent;

    // Thrust
    // T = W * g * eta_t
    // W = weight
    // g = gravity
    // eta_t = efficiency
    auto const thrust = weight * si::standard_gravity * eta_t;

    // Power-Vertical
    // P_v = (T x v) / eta_p
    // v = vertical speed
    // eta_p = aerodynamic efficiency
    auto const powerVertical0 = thrust * v_v / eta_p;
    auto const powerVertical  = powerVertical0 * sqrt(densityAt(0.0 * m) / rho);

    // Power-Horizontal
    // P_h = 0.5 x C_D x A_f x rho v_h^3
    // C_D = drag coefficient
    // A_f = frontal area
    // rho = air density
    // v_h = speed horizontal
    auto const vh3             = v_h * v_h * v_h;
    auto const powerHorizontal = 0.5 * C_D * A_f * rho * vh3;

    // Power-Total
    QuantityOf<isq::power> auto power                  = powerHorizontal + powerVertical;
    QuantityOf<isq::power / isq::mass> auto powerRatio = power / weight;
    QuantityOf<isq::energy> auto energy                = power * flightTime;

    fmt::println("Quad-Copter Flight:");
    fmt::println("------------------");
    fmt::println("Weight:      {::N[.3f]}", weight.in(g));
    fmt::println("Area_f:      {::N[.3f]}", A_f.in(m2));
    fmt::println("Thrust:      {::N[.3f]}\n", thrust.in(N));

    fmt::println("Distance:    {::N[.3f]}", distance.in(km));
    fmt::println("Altitude:    {::N[.3f]}", altitude.in(m));
    fmt::println("Air-Density: {::N[.3f]}\n", rho.in(kg / m3));

    fmt::println("Speed_v:     {::N[.3f]}", v_v.in(m / s));
    fmt::println("Speed_h:     {::N[.3f]}\n", v_h.in(km / h));

    fmt::println("Power_v:     {::N[.3f]}", powerVertical.in(W));
    fmt::println("Power_h:     {::N[.3f]}", powerHorizontal.in(W));
    fmt::println("Power_t:     {::N[.3f]}", power.in(W));
    fmt::println("Power-Ratio: {::N[.3f]}\n", powerRatio.in(W / kg));

    fmt::println("Time:        {::N[.3f]}", flightTime.in(h));
    fmt::println("Energy:      {::N[.3f]}", energy.in(kW * h));
    fmt::println("");
}

}  // namespace tdr
