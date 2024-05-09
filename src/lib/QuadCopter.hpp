#pragma once

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tug
{

using namespace mp_units;

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

struct QuadCopter
{
    quantity<isq::mass[si::kilogram]> weight;
    quantity<isq::area[square(si::metre)]> frontalArea;
    quantity<isq::maximum_efficiency[percent]> thrustEfficiency;
    quantity<isq::maximum_efficiency[percent]> aerodynamicEfficiency;
};

auto estimatePowerConsumption(QuadCopter const& copter, Flight const& flight) -> void;

}  // namespace tug
