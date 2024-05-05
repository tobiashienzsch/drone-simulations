#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

struct QuadCopter
{
    quantity<si::kilo<si::gram>> weight;
    quantity<si::metre / si::second> speed;
    quantity<square(si::metre)> frontalArea;
    quantity<one> thrustEfficiency;
    quantity<one> aerodynamicEfficiency;
};

inline auto powerConsumption(QuadCopter const& copter) -> void
{
    using namespace mp_units::si::unit_symbols;

    QuantityOf<isq::mass> auto weight              = copter.weight;
    QuantityOf<isq::area> auto A_f                 = copter.frontalArea;
    QuantityOf<isq::speed> auto v_h                = copter.speed;
    QuantityOf<isq::speed> auto v_v                = 10.0 * m / s;
    QuantityOf<isq::density> auto rho              = 1.225 * kg / m3;
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
    auto const powerVertical = thrust * v_v / eta_p;

    // Power-Horizontal
    // P_h = 0.5 x C_D x A_f x rho v_h^3
    // C_D = drag coefficient
    // A_f = frontal area
    // rho = air density
    // v_h = speed horizontal
    auto const vh3             = v_h * v_h * v_h;
    auto const powerHorizontal = 0.5 * C_D * A_f * rho * vh3;

    // Power-Total
    auto const powerTotal = powerHorizontal + powerVertical;

    fmt::println("Quad-Copter:");
    fmt::println("-----------");
    fmt::println("Weight:  {::N[.3f]}", weight.in(g));
    fmt::println("Area_f:  {::N[.3f]}", A_f.in(m2));
    fmt::println("Thrust:  {::N[.3f]}", thrust.in(N));
    fmt::println("Speed_v: {::N[.3f]}", v_v.in(m / s));
    fmt::println("Speed_h: {::N[.3f]}", v_h.in(km / h));
    fmt::println("Power_v: {::N[.3f]}", powerVertical.in(W));
    fmt::println("Power_h: {::N[.3f]}", powerHorizontal.in(W));
    fmt::println("Power_t: {::N[.3f]}", powerTotal.in(W));
    fmt::println("");
}

struct Drone
{
    quantity<si::kilo<si::gram>> takeoffWeight;
    quantity<si::metre / si::second> speed;
    quantity<one> efficiency;
};

inline auto dronePowerConsumption(Drone drone) -> void
{
    using namespace mp_units::si::unit_symbols;

    auto gravity = 1.0 * si::standard_gravity;

    QuantityOf<isq::length> auto distance = 3000.0 * km;
    QuantityOf<isq::time> auto flightTime = distance / drone.speed;

    QuantityOf<isq::mass> auto thrustToWeight = 1.5 * kg;
    QuantityOf<isq::force> auto thrust        = thrustToWeight * gravity;
    QuantityOf<isq::velocity> auto velocity   = 10.0 * m / s;

    QuantityOf<isq::power> auto power                  = thrust * velocity / drone.efficiency;
    QuantityOf<isq::power / isq::mass> auto powerRatio = power / (1.0 * kg);
    QuantityOf<isq::power> auto totalPower             = powerRatio * drone.takeoffWeight;

    QuantityOf<isq::energy> auto energy = totalPower * flightTime;

    fmt::println("Drone power consumption:");
    fmt::println("------------------------");
    fmt::println("Weight:      {}", drone.takeoffWeight.in(kg));
    fmt::println("Efficiency:  {}", drone.efficiency);
    fmt::println("Gravity:     {}\n", gravity.in(m / s2));

    fmt::println("Thrust:      {}\n", thrust.in(N));

    fmt::println("Velocity:    {}\n", velocity.in(km / h));

    fmt::println("Power:       {}", totalPower.in(W));
    fmt::println("Power-Ratio: {}\n", powerRatio.in(W / kg));

    fmt::println("Time:        {}", flightTime.in(h));
    fmt::println("Energy:      {}", energy.in(kW * h));
    fmt::println("");
}

}  // namespace tdr
