// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_CHECK_NON_FINITE_HPP
#define BOOST_DECIMAL_DETAIL_CHECK_NON_FINITE_HPP

#include <type_traits>
#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/type_traits.hpp>

namespace boost {
namespace decimal {
namespace detail {

// Prioritizes checking for nans and then checks for infs
template <typename Decimal>
constexpr auto check_non_finite(Decimal lhs, Decimal rhs) noexcept
    -> std::enable_if_t<is_decimal_floating_point_v<Decimal>, Decimal>
{
    constexpr Decimal zero {0, 0};

    if (isnan(lhs))
    {
        return lhs;
    }
    else if (isnan(rhs))
    {
        return rhs;
    }

    if (isinf(lhs))
    {
        return lhs;
    }
    else if (isinf(rhs))
    {
        return rhs;
    }

    return zero;
}

} //namespace detail
} //namespace decimal
} //namespace boost

#endif //BOOST_DECIMAL_DETAIL_CHECK_NON_FINITE_HPP
