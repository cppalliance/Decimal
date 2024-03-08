//  (C) Copyright John Maddock 2006.
//  (C) Copyright Matt Borland 2024.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DECIMAL_DETAIL_CMATH_LEGENDRE_HPP
#define BOOST_DECIMAL_DETAIL_CMATH_LEGENDRE_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/concepts.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/promotion.hpp>
#include <boost/decimal/detail/cmath/log1p.hpp>
#include <utility>
#include <type_traits>
#include <limits>

namespace boost {
namespace decimal {

namespace detail {

template <typename T1, typename T2, typename T3>
constexpr auto legendre_next(unsigned l, T1 x, T2 Pl, T3 Plm1) noexcept
{
    using result_type = promote_args_t<T1, T2, T3>;
    return ((2 * l + 1) * static_cast<result_type>(x) * static_cast<result_type>(Pl) - l * static_cast<result_type>(Plm1)) / (l + 1);
}

// Implement Legendre P and Q polynomials via recurrence:
template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE T>
constexpr T legendre_impl(unsigned l, T x, bool second = false) noexcept
{
    if (x < -1 || x > 1 || l > 128)
    {
        return std::numeric_limits<T>::signaling_NaN();
    }
    else if (isnan(x))
    {
        return x;
    }

    T p0 {1};
    T p1 {x};

    if (l == 0)
    {
        return p0;
    }

    unsigned n = 1;

    while (n < l)
    {
        std::swap(p0, p1);
        p1 = static_cast<T>(legendre_next(n, x, p0, p1));
        ++n;
    }

    return p1;
}

} //namespace detail

template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE T>
constexpr auto legendre(unsigned n, T x) noexcept -> std::enable_if_t<detail::is_decimal_floating_point_v<T>, T>
{
    return detail::legendre_impl(n, x);
}

} //namespace decimal
} //namespace boost

#endif //BOOST_DECIMAL_DETAIL_CMATH_LEGENDRE_HPP
