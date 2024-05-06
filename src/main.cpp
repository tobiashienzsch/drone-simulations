#include "Atmosphere.hpp"
#include "Hydrogen.hpp"
#include "Microgreens.hpp"
#include "QuadCopter.hpp"
#include "SolarPanel.hpp"

#include <mp-units/systems/cgs.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

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

    static constexpr auto location = tdr::SolarPanelLocation{
        .irradiance = 1'000.0 * W / m2,
        .daylight   = 12.0 * h,
    };

    static constexpr auto panel = tdr::SolarPanel{
        .width      = 12.0 * m,
        .height     = 2.0 * m,
        .efficiency = 18.0 * percent,
    };

    tdr::powerOutput(panel, location);

    static constexpr auto light = tdr::GrowLight{
        .power      = 15.0 * W,
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

    report(tdr::GrowContainer{
        .container      = container,
        .rack           = rack,
        .light          = light,
        .rows           = 2 * one,
        .lightsPerShelf = 2 * one,
    });
    report(tdr::Microgreen{
        .price       = 18.0 * EUR / kg,
        .seeds       = 13.0 * g,
        .water       = 0.25 * l / d,
        .light       = 8.0 * h / d,
        .germination = 0.0 * d,
        .grow        = 10.0 * d,
        .rest        = 2.0 * d,
        .harvist     = 325.0 * g,
        .msrp        = 13.0 * EUR / kg,
    });

    return EXIT_SUCCESS;
}
