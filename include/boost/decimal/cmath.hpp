// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_CMATH_HPP
#define BOOST_DECIMAL_CMATH_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <cmath>

namespace boost { namespace decimal {

template <typename T, std::enable_if_t<detail::is_decimal_floating_point_v<T>, bool> = true>
constexpr auto fmax(T lhs, T rhs) noexcept
{
    if (isnan(lhs) && !isnan(rhs))
    {
        return rhs;
    }
    else if ((!isnan(lhs) && isnan(rhs)) ||
             (isnan(lhs) && isnan(rhs)))
    {
        return lhs;
    }

    return lhs > rhs ? lhs : rhs;
}

}} // Namespaces

#endif // BOOST_DECIMAL_CMATH_HPP
