#pragma once

#include <mp-units/framework.h>

namespace tdr
{

namespace finance
{

// clang-format off
inline constexpr struct dim_currency : mp_units::base_dimension<"$"> {} dim_currency;
inline constexpr struct currency : mp_units::quantity_spec<currency, dim_currency> {} currency;

inline constexpr struct euro : mp_units::named_unit<"EUR", mp_units::kind_of<currency>> {} euro;
inline constexpr struct us_dollar : mp_units::named_unit<"USD", mp_units::kind_of<currency>> {} us_dollar;
// clang-format on

namespace unit_symbols
{

inline constexpr auto EUR = euro;
inline constexpr auto USD = us_dollar;

}  // namespace unit_symbols

}  // namespace finance

}  // namespace tdr
