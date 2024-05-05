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
    // tdr::powerOutput(tdr::SolarPanel{
    //     .area       = 100.0 * square(cm),
    //     .irradiance = 1'000.0 * W / m2,
    //     .efficiency = 18.0 * percent,
    // });

    static constexpr auto uav = tdr::QuadCopter{
        .weight                = 10.0 * kg,
        .frontalArea           = 30.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    };

    static constexpr auto flight = tdr::Flight{
        .distance = 3'000.0 * km,
        .altitude = 1'000.0 * m,
        .speed    = 100.0 * km / h,
    };

    tdr::estimatePowerConsumption(uav, flight.withAltitude(500.0 * m));
    tdr::estimatePowerConsumption(uav, flight.withAltitude(1'000.0 * m));
    tdr::estimatePowerConsumption(uav, flight.withAltitude(1'500.0 * m));

    tdr::hydrogenEnergyIn(5.0 * l);
    tdr::hydrogenEnergyIn(10.0 * l);
    tdr::hydrogenEnergyIn(25.0 * l);

    return EXIT_SUCCESS;
}
