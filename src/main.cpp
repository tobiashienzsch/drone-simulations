#include <mp-units/ostream.h>
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

static auto dronePowerConsumption() -> void
{
    constexpr auto const speed      = 150.0 * km / h;
    constexpr auto const distance   = 3000.0 * km;
    constexpr auto const flightTime = distance / speed;

    constexpr auto const motors         = 4;
    constexpr auto const motorPeakPower = 125.0 * W;
    constexpr auto const totalWatts     = motorPeakPower * motors;
    constexpr auto const energyUsed     = totalWatts * flightTime;

    std::cout << "Drone power consumption:\n";
    std::cout << "------------------------\n";
    std::cout << "WATTS: " << totalWatts.in(kW) << '\n';
    std::cout << "TIME: " << flightTime << '\n';
    std::cout << "ENERGY: " << energyUsed.in(kW * h) << '\n';
    std::cout << '\n';
}

static auto hydrogenEnergyPerVolume() -> void
{
    constexpr auto const volume        = 5.0 * l;
    constexpr auto const densityGas    = 0.08988 * kg / m3;
    constexpr auto const densityLiquid = 70.85 * kg / m3;

    auto const energyGas    = hydrogenEnergy(densityGas, volume);
    auto const energyLiquid = hydrogenEnergy(densityLiquid, volume);

    std::cout << "Hydrogen energy per volume:\n";
    std::cout << "---------------------------\n";
    std::cout << "VOLUME:         " << volume << '\n';
    std::cout << "DENSITY-GAS:    " << densityGas << '\n';
    std::cout << "DENSITY-LIQUID: " << densityLiquid << '\n';
    std::cout << "MASS-GAS:       " << (densityGas * volume).in(g) << '\n';
    std::cout << "MASS-LIQUID:    " << (densityLiquid * volume).in(g) << '\n';
    std::cout << "ENERGY-GAS:     " << energyGas.in(kW * h) << '\n';
    std::cout << "ENERGY-LIQUID:  " << energyLiquid.in(kW * h) << '\n';
    std::cout << "INCREASE:       " << energyLiquid / energyGas << "x\n";
    std::cout << '\n';
}

auto main() -> int
{
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);

    dronePowerConsumption();
    hydrogenEnergyPerVolume();

    return EXIT_SUCCESS;
}
