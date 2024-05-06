#include "Atmosphere.hpp"
#include "Hydrogen.hpp"
#include "Microgreens.hpp"
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
    using namespace tdr::finance::unit_symbols;

    // tdr::compressGas();

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

    tdr::harvistReport({
        .seeds     = 13.0 * g,
        .price     = 18.0 * EUR / kg,
        .light     = 8.0 * h / d,
        .growPhase = 10.0 * d,
        .restPhase = 2.0 * d,
        .harvist   = 325.0 * g,
        .msrp      = 13.0 * EUR / kg,
    });

    static constexpr auto location = tdr::SolarPanelLocation{
        .irradiance = 1'000.0 * W / m2,
        .daylight   = 12.0 * h,
    };

    static constexpr auto panel = tdr::SolarPanel{
        .width      = 500.0 * cm,
        .height     = 100.0 * cm,
        .efficiency = 18.0 * percent,
    };

    tdr::powerOutput(panel, location);

    static constexpr auto light = tdr::GrowLight{
        .power      = 25.0 * W,
        .efficiency = 90.0 * percent,
    };

    static constexpr auto container = tdr::IntermodalContainer{
        .length = 12.032 * m,
        .width  = 2.352 * m,
        .height = 2.385 * m,
    };
    static constexpr auto rack = tdr::GrowRack{
        .depth  = 0.5 * m,
        .width  = 1.0 * m,
        .height = 2.0 * m,
        .shelfs = 5 * one,
        .tray   = 25.0 * cm,
    };

    tdr::growContainer(container, rack, light);

    return EXIT_SUCCESS;
}
