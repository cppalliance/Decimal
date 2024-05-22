// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_CMATH_TRUNC_HPP
#define BOOST_DECIMAL_DETAIL_CMATH_TRUNC_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/detail/concepts.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/fenv_rounding.hpp>
#include <boost/decimal/detail/cmath/floor.hpp>
#include <boost/decimal/detail/cmath/ceil.hpp>
#include <boost/decimal/detail/cmath/frexp10.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <type_traits>
#include <limits>
#endif

namespace boost {
namespace decimal {

BOOST_DECIMAL_EXPORT template <typename T>
constexpr auto trunc(T val) noexcept
    BOOST_DECIMAL_REQUIRES(detail::is_decimal_floating_point_v, T)
{
    return (val > 0) ? floor(val) : ceil(val);
}

BOOST_DECIMAL_EXPORT template <typename T>
constexpr auto trunc(T val, int precision) noexcept
    BOOST_DECIMAL_REQUIRES(detail::is_decimal_floating_point_v, T)
{
    constexpr auto biggest_val {1 / std::numeric_limits<T>::epsilon()};

    if (precision == 0)
    {
        return trunc(val);
    }
    else if (isnan(val) || isinf(val) || abs(val) == 0 || val > biggest_val)
    {
        return val;
    }

    int exp {};
    auto sig {frexp10(val, &exp)};
    const auto isneg {val < 0};
    auto sig_dig {detail::num_digits(sig)};

    if (sig_dig <= precision)
    {
        return val;
    }

    if (sig_dig > precision + 1)
    {
        const auto digits_to_remove {sig_dig - (precision + 1)};
        sig /= detail::pow10(static_cast<typename T::significand_type>(digits_to_remove));
        exp += digits_to_remove;
        sig_dig -= digits_to_remove;
    }

    if (sig_dig > precision)
    {
        exp += detail::fenv_round(sig, isneg);
    }

    return {sig, exp, isneg};
}

} // namespace decimal
} // namespace boost

#endif // BOOST_DECIMAL_DETAIL_CMATH_TRUNC_HPP
