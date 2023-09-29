// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_FROM_CHARS_INTEGER_IMPL_HPP
#define BOOST_DECIMAL_DETAIL_FROM_CHARS_INTEGER_IMPL_HPP

#include <boost/decimal/detail/apply_sign.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/from_chars_result.hpp>
#include <boost/decimal/detail/emulated128.hpp>
#include <system_error>
#include <type_traits>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cstddef>
#include <cstdint>

namespace boost { namespace decimal { namespace detail {

static constexpr unsigned char uchar_values[] =
     {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 255, 255, 255, 255, 255, 255,
      255,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
       25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35, 255, 255, 255, 255, 255,
      255,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
       25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

static_assert(sizeof(uchar_values) == 256, "uchar_values should represent all 256 values of unsigned char");

// Convert characters for 0-9, A-Z, a-z to 0-35. Anything else is 255
constexpr auto digit_from_char(char val) noexcept -> unsigned char
{
    return uchar_values[static_cast<unsigned char>(val)];
}

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4146) // unary minus operator applied to unsigned type, result still unsigned
# pragma warning(disable: 4189) // 'is_negative': local variable is initialized but not referenced

#elif defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wconstant-conversion"

#elif defined(__GNUC__) && (__GNUC__ < 7)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Woverflow"

#elif defined(__GNUC__) && (__GNUC__ >= 7)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#endif

template <typename Integer, typename Unsigned_Integer>
constexpr auto from_chars_integer_impl(const char* first, const char* last, Integer& value, int base) noexcept -> from_chars_result
{
    Unsigned_Integer result = 0;
    Unsigned_Integer overflow_value = 0;
    Unsigned_Integer max_digit = 0;

    const auto unsigned_base = static_cast<Unsigned_Integer>(base);

    // Strip sign if the type is signed
    // Negative sign will be appended at the end of parsing
    BOOST_ATTRIBUTE_UNUSED bool is_negative = false;
    auto next = first;

    #ifdef BOOST_DECIMAL_HAS_INT128
    BOOST_DECIMAL_IF_CONSTEXPR (std::is_same<Integer, boost::int128_type>::value || std::is_signed<Integer>::value)
    #else
    BOOST_DECIMAL_IF_CONSTEXPR (std::is_signed<Integer>::value)
    #endif
    {
        if (next != last)
        {
            if (*next == '-')
            {
                is_negative = true;
                ++next;
            }
        }

        #ifdef BOOST_DECIMAL_HAS_INT128
        BOOST_DECIMAL_IF_CONSTEXPR (std::is_same<Integer, boost::int128_type>::value)
        {
            overflow_value = BOOST_DECIMAL_INT128_MAX;
            max_digit = BOOST_DECIMAL_INT128_MAX;
        }
        else
        #endif
        {
            overflow_value = (std::numeric_limits<Integer>::max)();
            max_digit = (std::numeric_limits<Integer>::max)();
        }

        if (is_negative)
        {
            ++overflow_value;
            ++max_digit;
        }
    }
    else
    {
        if (next != last && (*next == '-' || *next == '+'))
        {
            return {first, std::errc::invalid_argument};
        }
        
        #ifdef BOOST_DECIMAL_HAS_INT128
        BOOST_DECIMAL_IF_CONSTEXPR (std::is_same<Integer, boost::uint128_type>::value)
        {
            overflow_value = BOOST_DECIMAL_UINT128_MAX;
            max_digit = BOOST_DECIMAL_UINT128_MAX;
        }
        else
        #endif
        {
            overflow_value = (std::numeric_limits<Unsigned_Integer>::max)();
            max_digit = (std::numeric_limits<Unsigned_Integer>::max)();
        }
    }

    #ifdef BOOST_DECIMAL_HAS_INT128
    BOOST_DECIMAL_IF_CONSTEXPR (std::is_same<Integer, boost::int128_type>::value)
    {
        overflow_value /= unsigned_base;
        max_digit %= unsigned_base;
        overflow_value *= 2; // Overflow value would cause INT128_MIN in non-base10 to fail
    }
    else
    #endif
    {
        overflow_value /= unsigned_base;
        max_digit %= unsigned_base;
    }

    // If the only character was a sign abort now
    if (next == last)
    {
        return {first, std::errc::invalid_argument};
    }

    bool overflowed = false;

    std::ptrdiff_t nc = last - next;
    constexpr std::ptrdiff_t nd = std::numeric_limits<Integer>::digits10;

    {
        std::ptrdiff_t i = 0;

        for( ; i < nd && i < nc; ++i )
        {
            // overflow is not possible in the first nd characters

            const unsigned char current_digit = digit_from_char(*next);

            if (current_digit >= unsigned_base)
            {
                break;
            }

            result = static_cast<Unsigned_Integer>(result * unsigned_base + current_digit);
            ++next;
        }

        for( ; i < nc; ++i )
        {
            const unsigned char current_digit = digit_from_char(*next);

            if (current_digit >= unsigned_base)
            {
                break;
            }

            if (result < overflow_value || (result == overflow_value && current_digit <= max_digit))
            {
                result = static_cast<Unsigned_Integer>(result * unsigned_base + current_digit);
            }
            else
            {
                // Required to keep updating the value of next, but the result is garbage
                overflowed = true;
            }

            ++next;
        }
    }

    // Return the parsed value, adding the sign back if applicable
    // If we have overflowed then we do not return the result 
    if (overflowed)
    {
        return {next, std::errc::result_out_of_range};
    }

    value = static_cast<Integer>(result);
    #ifdef BOOST_DECIMAL_HAS_INT128
    BOOST_DECIMAL_IF_CONSTEXPR (std::is_same<Integer, boost::int128_type>::value || std::is_signed<Integer>::value)
    #else
    BOOST_DECIMAL_IF_CONSTEXPR (std::is_signed<Integer>::value)
    #endif
    {
        if (is_negative)
        {
            value = static_cast<Integer>(-(static_cast<Unsigned_Integer>(value)));
        }
    }

    return {next, std::errc()};
}

#ifdef BOOST_MSVC
# pragma warning(pop)
#elif defined(__clang__) && defined(__APPLE__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

// Only from_chars for integer types is constexpr (as of C++23)
template <typename Integer>
constexpr auto from_chars(const char* first, const char* last, Integer& value, int base = 10) noexcept -> from_chars_result
{
    using Unsigned_Integer = typename std::make_unsigned_t<Integer>;
    return detail::from_chars_integer_impl<Integer, Unsigned_Integer>(first, last, value, base);
}

#ifdef BOOST_DECIMAL_HAS_INT128
template <typename Integer>
constexpr from_chars_result from_chars128(const char* first, const char* last, Integer& value, int base = 10) noexcept
{
    using Unsigned_Integer = uint128_t;
    return detail::from_chars_integer_impl<Integer, Unsigned_Integer>(first, last, value, base);
}
#endif

constexpr auto from_chars128(const char* first, const char* last, uint128& value, int base = 10) noexcept -> from_chars_result
{
    return from_chars_integer_impl<uint128, uint128>(first, last, value, base);
}

} // namespace detail
} // namespace decimal
} // namespace boost

#endif // BOOST_DECIMAL_DETAIL_FROM_CHARS_INTEGER_IMPL_HPP
