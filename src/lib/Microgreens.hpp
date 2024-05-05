#pragma once

#include <fmt/format.h>
#include <fmt/os.h>

#include <mp-units/format.h>
#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

using namespace mp_units;

namespace tdr
{

namespace finance
{

// clang-format off
inline constexpr struct dim_currency : base_dimension<"$"> {} dim_currency;
inline constexpr struct currency : quantity_spec<currency, dim_currency> {} currency;

inline constexpr struct euro : named_unit<"EUR", kind_of<currency>> {} euro;
inline constexpr struct us_dollar : named_unit<"USD", kind_of<currency>> {} us_dollar;
// clang-format on

namespace unit_symbols
{

inline constexpr auto EUR = euro;
inline constexpr auto USD = us_dollar;

}  // namespace unit_symbols

}  // namespace finance

struct Microgreen
{
    quantity<si::gram> seeds;
    quantity<finance::euro / si::kilo<si::gram>> price;

    quantity<one> light;
    quantity<si::day> growPhase;
    quantity<si::day> restPhase;

    quantity<si::gram> harvist;
    quantity<finance::euro / si::kilo<si::gram>> msrp;
};

inline auto harvistReport(Microgreen const& plant) -> void
{
    using namespace mp_units::si::unit_symbols;
    using namespace finance::unit_symbols;

    QuantityOf<dimensionless> auto light = plant.light;
    QuantityOf<isq::time> auto growPhase = plant.growPhase;
    QuantityOf<isq::time> auto restPhase = plant.restPhase;
    QuantityOf<isq::mass> auto harvist   = plant.harvist;

    QuantityOf<finance::currency / isq::mass> auto msrp = plant.msrp;
    QuantityOf<finance::currency> auto price            = plant.price * plant.seeds;
    QuantityOf<finance::currency> auto value            = msrp * harvist;

    fmt::println("Microgreens:");
    fmt::println("-----------");
    fmt::println("Seeds:   {}", plant.seeds.in(g));
    fmt::println("Price:   {::N[.2f]}\n", price.in(EUR));

    fmt::println("Light:   {}", light.in(h / d));
    fmt::println("Grow:    {}", growPhase.in(d));
    fmt::println("Rest:    {}", restPhase.in(d));
    fmt::println("Cycle:   {}\n", (growPhase + restPhase).in(d));

    fmt::println("Harvist: {}", harvist.in(g));
    fmt::println("MSRP:    {}", msrp.in(EUR / kg));
    fmt::println("Value:   {::N[.2f]}", value.in(EUR));
    fmt::println("Profit:  {::N[.2f]}", (value - price).in(EUR));
    fmt::println("");
}

}  // namespace tdr
