#include "Atmosphere.hpp"
#include "Hydrogen.hpp"
#include "QuadCopter.hpp"
#include "SolarPanel.hpp"

#include <mp-units/ostream.h>
#include <mp-units/systems/cgs.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include <iostream>

auto main() -> int
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    // tdr::compressGas();
    // tdr::hydrogenEnergyPerVolume();
    // tdr::powerOutput(tdr::SolarPanel{
    //     .area       = 100.0 * square(cm),
    //     .irradiance = 1'000.0 * W / m2,
    //     .efficiency = 18.0 * percent,
    // });

    constexpr auto const uav = tdr::QuadCopter{
        .weight                = 2.0 * kg,
        .speed                 = 200.0 * km / h,
        .frontalArea           = 10.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    };

    tdr::powerConsumption(uav, {.distance = 3'000.0 * km, .altitude = 0.0 * m});
    tdr::powerConsumption(uav, {.distance = 3'000.0 * km, .altitude = 1'000.0 * m});
    tdr::powerConsumption(uav, {.distance = 3'000.0 * km, .altitude = 2'000.0 * m});

    return EXIT_SUCCESS;
}
