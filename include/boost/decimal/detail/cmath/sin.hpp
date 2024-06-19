// Copyright 2023 - 2024 Matt Borland
// Copyright 2023 - 2024 Christopher Kormanyos
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_CMATH_SIN_HPP
#define BOOST_DECIMAL_DETAIL_CMATH_SIN_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/numbers.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/detail/concepts.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/cmath/cos.hpp>
#include <boost/decimal/detail/cmath/remquo.hpp>
#include <boost/decimal/detail/cmath/impl/sin_impl.hpp>
#include <boost/decimal/detail/cmath/impl/cos_impl.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <type_traits>
#include <cstdint>
#endif

namespace boost {
namespace decimal {

namespace detail {

template <typename T>
constexpr auto sin_impl(T x) noexcept
    BOOST_DECIMAL_REQUIRES(detail::is_decimal_floating_point_v, T)
{
    T result { };

    const auto fpc = fpclassify(x);

    // First check non-finite values and small angles.
    if (fabs(x) < std::numeric_limits<T>::epsilon() || (fpc == FP_INFINITE) || (fpc == FP_NAN))
    {
        result = x;
    }
    else if (signbit(x))
    {
        result = -sin(-x);
    }
    else
    {
        if(x > static_cast<int>(INT8_C(0)))
        {
            // Perform argument reduction and subsequent scaling of the result.

            // Given x = k * (pi/2) + r, compute n = (k % 4).

            // | n |  sin(x) |  cos(x) |  sin(x)/cos(x) |
            // |----------------------------------------|
            // | 0 |  sin(r) |  cos(r) |  sin(r)/cos(r) |
            // | 1 |  cos(r) | -sin(r) | -cos(r)/sin(r) |
            // | 2 | -sin(r) | -cos(r) |  sin(r)/cos(r) |
            // | 3 | -cos(r) |  sin(r) | -cos(r)/sin(r) |

            constexpr T my_pi_half { numbers::pi_v<T> / 2 };

            const auto k = static_cast<unsigned>(x / my_pi_half);
            const auto n = static_cast<unsigned>(k % static_cast<unsigned>(UINT8_C(4)));

            auto r = x - (my_pi_half * k);

            constexpr T half { 5, -1 };

            const bool do_scaling { x > half };

            if(do_scaling)
            {
                // Reduce the argument with factors of three.
                r /= static_cast<unsigned>(UINT8_C(3));
            }

            switch(n)
            {
              case static_cast<unsigned>(UINT8_C(1)):
              case static_cast<unsigned>(UINT8_C(3)):
                result = detail::cos_series_expansion(r);
                break;
              case static_cast<unsigned>(UINT8_C(0)):
              case static_cast<unsigned>(UINT8_C(2)):
              default:
                result = detail::sin_series_expansion(r);
                break;
            }

            if(do_scaling)
            {
                result *= (static_cast<unsigned>(UINT8_C(3)) - ((result * result) * static_cast<unsigned>(UINT8_C(4))));
            }

            if(signbit(result)) { result = -result; }

            const auto b_neg = (n > static_cast<unsigned>(UINT8_C(1)));

            if(b_neg) { result = -result; }
        }
    }

    return result;
}

} // namespace detail

BOOST_DECIMAL_EXPORT template <typename T>
constexpr auto sin(T x) noexcept
    BOOST_DECIMAL_REQUIRES(detail::is_decimal_floating_point_v, T)
{
    #if BOOST_DECIMAL_DEC_EVAL_METHOD == 0

    using evaluation_type = T;

    #elif BOOST_DECIMAL_DEC_EVAL_METHOD == 1

    using evaluation_type = detail::promote_args_t<T, decimal64>;

    #else // BOOST_DECIMAL_DEC_EVAL_METHOD == 2

    using evaluation_type = detail::promote_args_t<T, decimal128>;

    #endif

    return static_cast<T>(detail::sin_impl(static_cast<evaluation_type>(x)));
}

} // namespace decimal
} // namespace boost


#endif // BOOST_DECIMAL_DETAIL_CMATH_SIN_HPP
