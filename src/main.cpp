#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/systems/international.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

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

    fmt::println("Drone power consumption:");
    fmt::println("------------------------");
    fmt::println("Weight:      {}", drone.takeoffWeight.in(kg));
    fmt::println("Efficiency:  {}", drone.efficiency);
    fmt::println("Gravity:     {}\n", gravity.in(m / s2));

    fmt::println("Thrust:      {}\n", thrust.in(N));

    fmt::println("Velocity:    {}\n", velocity.in(km / h));

    fmt::println("Power:       {}", totalPower.in(W));
    fmt::println("Power-Ratio: {}\n", powerRatio.in(W / kg));

    fmt::println("Time:        {}", flightTime.in(h));
    fmt::println("Energy:      {}", energy.in(kW * h));
    fmt::println("");
}

static auto hydrogenEnergyPerVolume() -> void
{
    QuantityOf<isq::volume> auto volume         = 5.0 * l;
    QuantityOf<isq::density> auto densityGas    = 0.08988 * kg / m3;
    QuantityOf<isq::density> auto densityLiquid = 70.85 * kg / m3;

    QuantityOf<isq::energy> auto energyGas    = hydrogenEnergy(densityGas, volume);
    QuantityOf<isq::energy> auto energyLiquid = hydrogenEnergy(densityLiquid, volume);

    fmt::println("Hydrogen energy per volume:");
    fmt::println("---------------------------");
    fmt::println("Volume:         {}", volume);
    fmt::println("Density Gas:    {}", densityGas);
    fmt::println("Mass Gas:       {}", (densityGas * volume).in(g));
    fmt::println("Energy Gas:     {}\n", energyGas.in(kW * h));

    fmt::println("Density Liquid: {}", densityLiquid);
    fmt::println("Mass Liquid:    {}", (densityLiquid * volume).in(g));
    fmt::println("Energy Liquid:  {}\n", energyLiquid.in(kW * h));

    fmt::println("Increase:       {}x", energyLiquid / energyGas);
    fmt::println("");
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

    auto R = (1.0 * si::si2019::boltzmann_constant * si::si2019::avogadro_constant).in(J / (mol * K));

    QuantityOf<isq::pressure> auto P                                    = 200.0 * 100'000.0 * Pa;
    QuantityOf<isq::volume> auto V                                      = 5.0 * l;
    QuantityOf<isq::thermodynamic_temperature> auto T                   = 298.0 * K;
    QuantityOf<isq::amount_of_substance> auto moles                     = (P * V) / (R * T);
    QuantityOf<isq::mass / isq::amount_of_substance> auto molecularMass = 2.0 * g / mol;
    QuantityOf<isq::mass> auto mass                                     = moles * molecularMass;

    fmt::println("Compress gas:");
    fmt::println("-------------");
    fmt::println("Pressure:     {}", P);
    fmt::println("Volume:       {}", V);
    fmt::println("Gas Constant: {}", R);
    fmt::println("Temperature:  {}", T);
    fmt::println("Moles:        {}", moles);
    fmt::println("Mass:         {}", mass);
    fmt::println("");
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

    fmt::println("Solar panel:");
    fmt::println("------------");
    fmt::println("Area:       {}", panel.area.in(m2));
    fmt::println("Irradiance: {}", panel.irradiance);
    fmt::println("Efficiency: {}", panel.efficiency);
    fmt::println("Power:      {}", power.in(W));
    fmt::println("Energy:     {}", energy.in(W * h));
    fmt::println("");
}

auto main() -> int
{
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
