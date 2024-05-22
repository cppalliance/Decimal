// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_CMATH_FLOOR_HPP
#define BOOST_DECIMAL_DETAIL_CMATH_FLOOR_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/detail/power_tables.hpp>
#include <boost/decimal/detail/apply_sign.hpp>
#include <boost/decimal/detail/cmath/fpclassify.hpp>
#include <boost/decimal/detail/cmath/frexp10.hpp>
#include <boost/decimal/detail/emulated128.hpp>
#include <boost/decimal/detail/concepts.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <type_traits>
#include <cmath>
#endif

namespace boost {
namespace decimal {

BOOST_DECIMAL_EXPORT template <typename T>
constexpr auto floor BOOST_DECIMAL_PREVENT_MACRO_SUBSTITUTION (T val) noexcept
    BOOST_DECIMAL_REQUIRES(detail::is_decimal_floating_point_v, T)
{
    using DivType = typename T::significand_type;

    constexpr T zero {0, 0};
    constexpr T neg_one {1, 0, true};
    const auto fp {fpclassify(val)};

    switch (fp)
    {
        case FP_ZERO:
        case FP_NAN:
        case FP_INFINITE:
            return val;
        default:
            static_cast<void>(val);
    }

    int exp_ptr {};
    auto new_sig {frexp10(val, &exp_ptr)};
    const auto abs_exp {detail::make_positive_unsigned(exp_ptr)};
    const bool is_neg {val < zero};

    const auto sig_dig {static_cast<unsigned>(detail::num_digits(new_sig))};
    auto decimal_digits {sig_dig};
    bool round {false};

    if (sig_dig > abs_exp)
    {
        decimal_digits = abs_exp;
        if (sig_dig == abs_exp + 1)
        {
            round = true;
        }
    }
    else if (exp_ptr < 1 && abs_exp >= sig_dig)
    {
        return is_neg ? neg_one : zero;
    }
    else
    {
        decimal_digits--;
    }

    new_sig /= detail::pow10<DivType>(decimal_digits);
    if (is_neg && round)
    {
        ++new_sig;
    }

    return T{new_sig, exp_ptr + static_cast<int>(decimal_digits), is_neg};
}

} // namespace decimal
} // namespace boost

#endif //BOOST_DECIMAL_DETAIL_CMATH_FLOOR_HPP
