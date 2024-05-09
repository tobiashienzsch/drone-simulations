#include "Atmosphere.hpp"
#include "Hydrogen.hpp"
#include "Microgreens.hpp"
#include "QuadCopter.hpp"
#include "SolarPanel.hpp"

#include <mp-units/systems/cgs.h>
#include <mp-units/systems/international.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include <algorithm>

auto main(int argc, char const** argv) -> int
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;
    using namespace mp_units::international::unit_symbols;
    using namespace tdr::finance::unit_symbols;

    // tdr::compressGas();

    // static constexpr auto uav = tdr::QuadCopter{
    //     .weight                = 5.0 * kg,
    //     .frontalArea           = 10.0 * 30.0 * square(cm),
    //     .thrustEfficiency      = 130.0 * percent,
    //     .aerodynamicEfficiency = 70.0 * percent,
    // };

    // static constexpr auto flight = tdr::Flight{
    //     .distance = 3'000.0 * km,
    //     .altitude = 1'000.0 * m,
    //     .speed    = 120.0 * km / h,
    // };

    // tdr::estimatePowerConsumption(uav, flight.withAltitude(500.0 * m));
    // tdr::estimatePowerConsumption(uav, flight.withAltitude(1'000.0 * m));

    // tdr::hydrogenEnergyIn(5.0 * l);
    // tdr::hydrogenEnergyIn(10.0 * l);
    // tdr::hydrogenEnergyIn(25.0 * l);

    // static constexpr auto location = tdr::SolarPanel::Location{
    //     .irradiance = 1'269.0 * W / m2,
    //     .daylight   = 8.0 * h,
    // };

    // static constexpr auto panel = tdr::SolarPanel{
    //     .width      = 12.0 * m,
    //     .height     = 2.0 * m,
    //     .efficiency = 18.0 * percent,
    // };

    // tdr::powerOutput(panel, location);

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

    static constexpr auto gc = tdr::GrowContainer{
        .container      = container,
        .rack           = rack,
        .light          = light,
        .rows           = 2 * one,
        .lightsPerShelf = 2 * one,
    };

    static auto const plant = tdr::Microgreen{
        .name        = "Generic",
        .price       = 18.0 * EUR / kg,
        .seeds       = 13.0 * g,
        .water       = 0.25 * l / d,
        .light       = 8.0 * h / d,
        .germination = 0.0 * d,
        .grow        = 10.0 * d,
        .rest        = 2.0 * d,
        .yield       = 325.0 * g,
        .msrp        = 13.0 * EUR / kg,
    };

    static auto const basil = tdr::Microgreen{
        .name        = "Basil",
        .price       = 50.0 * EUR / kg,
        .seeds       = 6.5 * g,
        .water       = 0.25 * l / d,
        .light       = 8.0 * h / d,
        .germination = 0.0 * d,
        .grow        = 17.0 * d,
        .rest        = 2.0 * d,
        .yield       = 7.5 * oz,
        .msrp        = 15.0 * EUR / kg,
    };

    static auto const tokyoBekana = tdr::Microgreen{
        .name        = "Tokyo Bekana",
        .price       = 70.0 * EUR / kg,
        .seeds       = 14.0 * g,
        .water       = 0.25 * l / d,
        .light       = 8.0 * h / d,
        .germination = 0.0 * d,
        .grow        = 8.0 * d,
        .rest        = 2.0 * d,
        .yield       = 15.0 * oz,
        .msrp        = 12.0 * EUR / kg,
    };

    report(plant, gc);
    report(basil, gc);
    report(tokyoBekana, gc);
    report(gc);

    if (argc == 2)
    {
        auto plants = tdr::loadMicrogreens(argv[1]);
        auto less   = [](auto const& l, auto const& r) { return (l.yield / l.grow) < (r.yield / r.grow); };
        std::ranges::sort(plants, less);

        for (auto const& plant : plants)
        {
            fmt::println("---------------------------");
            fmt::println("{}", plant.name);
            report(plant, gc);
            // fmt::println("Price:            {::N[.2f]}", plant.price.in(EUR / kg));
            // fmt::println("Seeds:            {::N[.2f]}", plant.seeds.in(g));
            // fmt::println("Grow-Phase:       {::N[.2f]}", plant.grow.in(d));
            // fmt::println("Yield:            {::N[.2f]}", plant.yield.in(g));
            // fmt::println("Yield/Days:       {::N[.2f]}", (plant.yield / plant.grow).in(g / d));
            // fmt::println("Yield/Days/Seeds: {::N[.2f]}", plant.yield / plant.seeds / plant.grow);
            fmt::println("");
        }
    }

    return EXIT_SUCCESS;
}
