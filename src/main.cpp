#include "Hydrogen.hpp"
#include "QuadCopter.hpp"
#include "SolarPanel.hpp"

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

auto main() -> int
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    tdr::compressGas();
    tdr::hydrogenEnergyPerVolume();
    tdr::dronePowerConsumption({
        .takeoffWeight = 3.0 * kg,
        .speed         = 150.0 * km / h,
        .efficiency    = 80.0 * percent,
    });

    tdr::powerOutput(tdr::SolarPanel{
        .area       = 100.0 * square(cm),
        .irradiance = 1'000.0 * W / m2,
        .efficiency = 18.0 * percent,
    });

    tdr::powerConsumption({
        .weight                = 2.0 * kg,
        .speed                 = 100.0 * km / h,
        .frontalArea           = 10.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    });

    tdr::powerConsumption({
        .weight                = 4.0 * kg,
        .speed                 = 100.0 * km / h,
        .frontalArea           = 10.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    });

    return EXIT_SUCCESS;
}
