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

    tdr::compressGas();
    tdr::hydrogenEnergyPerVolume();

    // tdr::powerOutput(tdr::SolarPanel{
    //     .area       = 100.0 * square(cm),
    //     .irradiance = 1'000.0 * W / m2,
    //     .efficiency = 18.0 * percent,
    // });

    tdr::powerConsumption({
        .weight                = 2.0 * kg,
        .speed                 = 100.0 * km / h,
        .frontalArea           = 10.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    });

    tdr::powerConsumption({
        .weight                = 2.0 * kg,
        .speed                 = 200.0 * km / h,
        .frontalArea           = 10.0 * 30.0 * square(cm),
        .thrustEfficiency      = 130.0 * percent,
        .aerodynamicEfficiency = 70.0 * percent,
    });

    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(0.0 * m).in(tdr::atm), 0.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(100.0 * m).in(tdr::atm), 100.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(1000.0 * m).in(tdr::atm), 1000.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(2000.0 * m).in(tdr::atm), 2000.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(3000.0 * m).in(tdr::atm), 3000.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(4000.0 * m).in(tdr::atm), 4000.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(5000.0 * m).in(tdr::atm), 5000.0 * m);
    fmt::println("{::N[.3f]} at {}", tdr::pressureAt(10'000.0 * m).in(tdr::atm), 10'000.0 * m);

    return EXIT_SUCCESS;
}
