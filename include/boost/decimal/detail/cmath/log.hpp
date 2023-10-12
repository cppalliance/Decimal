// Copyright 2023 Matt Borland
// Copyright 2023 Christopher Kormanyos
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_CMATH_LOG_HPP
#define BOOST_DECIMAL_DETAIL_CMATH_LOG_HPP

#include <cmath>
#include <type_traits>

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/numbers.hpp>

namespace boost { namespace decimal {

template<typename T>
constexpr auto log(T x) noexcept -> std::enable_if_t<detail::is_decimal_floating_point_v<T>, T> // NOLINT(misc-no-recursion)
{
    constexpr T zero { 0, 0 };
    constexpr T one  { 1, 0 };

    auto result = zero;

    if (isnan(x))
    {
        result = x;
    }
    else if (isinf(x))
    {
        if (!signbit(x))
        {
            result = x;
        }
        else
        {
            result = std::numeric_limits<T>::quiet_NaN();
        }
    }
    else if (x < one)
    {
        // Handle reflection, the [+/-] zero-pole, and non-pole, negative x.
        if (x > zero)
        {
            result = -log(one / x);
        }
        else if ((x == zero) || (-x == zero))
        {
            // Actually, this should be equivalent to -HUGE_VAL.

            result = -std::numeric_limits<T>::infinity();
        }
        else
        {
            result = std::numeric_limits<T>::quiet_NaN();
        }
    }
    else if(x > one)
    {
        // The algorithm for logarithm is based on Chapter 5, pages 35-36
        // of Cody and Waite, Software Manual for the Elementary Functions,
        // Prentice Hall, 1980.

        int exp2val { };

        // TODO(ckormanyos) There is probably something more efficient than calling frexp here.
        auto g = frexp(x, &exp2val);

        if (g < numbers::inv_sqrt2_v<T>)
        {
            g += g;

            --exp2val;
        }

        using coefficient_array_type = std::array<T, static_cast<std::size_t>(UINT8_C(11))>;

        #if (defined(__clang__) && (__clang__ < 6))
        #  pragma GCC diagnostic push
        #  pragma GCC diagnostic ignored "-Wmissing-braces"
        #endif

        constexpr auto coefficient_table =
            coefficient_array_type
            {
                // (1,) 12, 80, 448, 2304, 11264, 53248, 245760, 1114112, 4980736, 22020096, 96468992, ...
                // See also Sloane's A058962 at: https://oeis.org/A058962

                // See also
                // Series[Log[(1 + (z/2))/(1 - (z/2))], {z, 0, 23}]
                // Or at Wolfram Alpha: https://www.wolframalpha.com/input?i=Series%5BLog%5B%281+%2B+%28z%2F2%29%29%2F%281+-+%28z%2F2%29%29%5D%2C+%7Bz%2C+0%2C+21%7D%5D

                T { UINT64_C(833333333333333333), -18 - 1 }, // * z^3
                T { UINT64_C(125000000000000000), -18 - 1 }, // * z^5
                T { UINT64_C(223214285714285714), -18 - 2 }, // * z^7
                T { UINT64_C(434027777777777778), -18 - 3 }, // * z^9
                T { UINT64_C(887784090909090909), -18 - 4 }, // * z^11
                T { UINT64_C(187800480769230769), -18 - 4 }, // * z^13
                T { UINT64_C(406901041666666667), -18 - 5 }, // * z^15
                T { UINT64_C(897575827205882353), -18 - 6 }, // * z^17
                T { UINT64_C(200773540296052632), -18 - 6 }, // * z^19
                T { UINT64_C(454130626860119048), -18 - 7 }, // * z^21
                T { UINT64_C(103660251783288043), -18 - 7 }  // * z^23
            };

        #if (defined(__clang__) && (__clang__ < 6))
        #  pragma GCC diagnostic pop
        #endif

        const auto s   = (g - one) / (g + one);
        const auto z   = s + s;
        const auto zsq = z * z;

        auto rit = coefficient_table.crbegin() + static_cast<std::size_t>((sizeof(T) == 4U) ? 5U : 0U);

        result = *rit;

        while(rit != coefficient_table.crend())
        {
            result = fma(result, zsq, *rit++);
        }

        result = z * fma(result, zsq, one);

        if (exp2val > 0)
        {
            result += static_cast<T>(exp2val * numbers::ln2_v<T>);
        }
    }
    else
    {
        result = zero;
    }

    return result;
}

} // namespace decimal
} // namespace boost

#endif // BOOST_DECIMAL_DETAIL_CMATH_LOG_HPP
