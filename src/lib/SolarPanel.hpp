#pragma once

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

namespace tug
{

using namespace mp_units;

struct SolarPanel
{
    struct Location
    {
        quantity<isq::irradiance[si::watt / square(si::metre)]> irradiance;
        quantity<isq::time[si::hour]> daylight;
    };

    quantity<isq::width[si::metre]> width;
    quantity<isq::height[si::metre]> height;
    quantity<isq::maximum_efficiency[percent]> efficiency;
};

auto report(SolarPanel const& panel, SolarPanel::Location const& location) -> void;

}  // namespace tug
