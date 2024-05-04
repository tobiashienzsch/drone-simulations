#include <mp-units/ostream.h>
#include <mp-units/systems/international.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include <iostream>

using namespace mp_units;
using namespace mp_units::si::unit_symbols;

constexpr auto hydrogenEnergy(QuantityOf<isq::mass_density> auto density,
                              QuantityOf<isq::volume> auto volume) -> QuantityOf<isq::energy> auto
{
    constexpr auto const lowerHeatingValue = 33.3 * kW * h / kg;
    return density * volume * lowerHeatingValue;
}

struct Drone
{
    quantity<si::kilo<si::gram>> takeoffWeight;
    quantity<si::metre / si::second> speed;
    quantity<one> efficiency;
};

static auto dronePowerConsumption(Drone drone) -> void
{
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

    std::cout << "Drone power consumption:\n";
    std::cout << "------------------------\n";
    std::cout << "Weight:      " << drone.takeoffWeight.in(kg) << '\n';
    std::cout << "Efficiency:  " << drone.efficiency << '\n';
    std::cout << "Gravity:     " << gravity.in(m / s2) << '\n' << '\n';
    std::cout << "Thrust:      " << thrust.in(N) << '\n' << '\n';
    std::cout << "Velocity:    " << velocity.in(km / h) << '\n' << '\n';

    std::cout << "Power:       " << totalPower.in(W) << '\n';
    std::cout << "Power-Ratio: " << powerRatio.in(W / kg) << '\n' << '\n';

    std::cout << "Time:        " << flightTime.in(h) << '\n';
    std::cout << "Energy:      " << energy.in(kW * h) << '\n';
    std::cout << '\n';
}

static auto hydrogenEnergyPerVolume() -> void
{
    QuantityOf<isq::volume> auto volume         = 5.0 * l;
    QuantityOf<isq::density> auto densityGas    = 0.08988 * kg / m3;
    QuantityOf<isq::density> auto densityLiquid = 70.85 * kg / m3;

    QuantityOf<isq::energy> auto energyGas    = hydrogenEnergy(densityGas, volume);
    QuantityOf<isq::energy> auto energyLiquid = hydrogenEnergy(densityLiquid, volume);

    std::cout << "Hydrogen energy per volume:\n";
    std::cout << "---------------------------\n";
    std::cout << "Volume:         " << volume << '\n';
    std::cout << "Density Gas:    " << densityGas << '\n';
    std::cout << "Mass Gas:       " << (densityGas * volume).in(g) << '\n';
    std::cout << "Energy Gas:     " << energyGas.in(kW * h) << '\n' << '\n';

    std::cout << "Density Liquid: " << densityLiquid << '\n';
    std::cout << "Mass Liquid:    " << (densityLiquid * volume).in(g) << '\n';
    std::cout << "Energy Liquid:  " << energyLiquid.in(kW * h) << '\n' << '\n';

    std::cout << "Increase:       " << energyLiquid / energyGas << "x\n";
    std::cout << '\n';
}

static auto compressGas() -> void
{
    // P = 200 * 100000  # Pressure in pascals (200 bar converted to Pascals)
    // V = 5  # Volume in liters
    // R = 8.314  # Universal gas constant in J/(mol·K)
    // T = 298  # Temperature in Kelvin (25 degrees Celsius)
    //
    // # Calculate moles of hydrogen
    // n = (P * V) / (R * T)

    QuantityOf<isq::pressure> auto P = 200.0 * 100'000.0 * Pa;  // 2e+7 * Pa;
    QuantityOf<isq::volume> auto V   = 5.0 * l;
    auto R = (1.0 * si::si2019::boltzmann_constant * si::si2019::avogadro_constant).in(J / (mol * K));
    QuantityOf<isq::thermodynamic_temperature> auto T                   = 298.0 * K;
    QuantityOf<isq::amount_of_substance> auto moles                     = (P * V) / (R * T);
    QuantityOf<isq::mass / isq::amount_of_substance> auto molecularMass = 2.0 * g / mol;
    QuantityOf<isq::mass> auto mass                                     = moles * molecularMass;

    std::cout << "Compress gas:\n";
    std::cout << "-------------\n";
    std::cout << "Pressure:     " << P << '\n';
    std::cout << "Volume:       " << V << '\n';
    std::cout << "Gas Constant: " << R << '\n';
    std::cout << "Temperature:  " << T << '\n';
    std::cout << "Moles:        " << moles << '\n';
    std::cout << "Mass:         " << mass << '\n';
    std::cout << '\n';
}

struct SolarPanel
{
    quantity<square(si::metre)> area;
    quantity<si::watt / square(si::metre)> irradiance;
    quantity<one> efficiency;
};

static auto powerOutput(SolarPanel panel) -> void
{
    QuantityOf<isq::power> auto power   = panel.area * panel.irradiance * panel.efficiency;
    QuantityOf<isq::time> auto duration = 5.0 * h;
    QuantityOf<isq::energy> auto energy = power * duration;

    std::cout << "Solar panel:\n";
    std::cout << "------------\n";
    std::cout << "Area:       " << panel.area.in(m2) << '\n';
    std::cout << "Irradiance: " << panel.irradiance << '\n';
    std::cout << "Efficiency: " << panel.efficiency << '\n';
    std::cout << "Power:      " << power.in(W) << '\n';
    std::cout << "Energy:     " << energy.in(W * h) << '\n';
    std::cout << '\n';
}

auto main() -> int
{
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);

    compressGas();
    hydrogenEnergyPerVolume();
    dronePowerConsumption({
        .takeoffWeight = 3.0 * kg,
        .speed         = 150.0 * km / h,
        .efficiency    = 80.0 * percent,
    });

    powerOutput(SolarPanel{
        .area       = 100.0 * square(cm),
        .irradiance = 1'000.0 * W / m2,
        .efficiency = 18.0 * percent,
    });

    return EXIT_SUCCESS;
}
