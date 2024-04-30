// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_FROM_CHARS_RESULT_HPP
#define BOOST_DECIMAL_DETAIL_FROM_CHARS_RESULT_HPP

#include <boost/decimal/detail/config.hpp>

#if !defined(BOOST_DECIMAL_DISABLE_CLIB)

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <system_error>
#endif

namespace boost {
namespace decimal {

// 22.13.3, Primitive numerical input conversion

BOOST_DECIMAL_EXPORT struct from_chars_result
{
    const char* ptr;

    // Values:
    // 0 = no error
    // EINVAL = invalid_argument
    // ERANGE = result_out_of_range
    std::errc ec;

    friend constexpr auto operator==(const from_chars_result& lhs, const from_chars_result& rhs) noexcept -> bool
    {
        return lhs.ptr == rhs.ptr && lhs.ec == rhs.ec;
    }

    friend constexpr auto operator!=(const from_chars_result& lhs, const from_chars_result& rhs) noexcept -> bool
    {
        return !(lhs == rhs); // NOLINT : Expression can not be simplified since this is the definition
    }

    constexpr explicit operator bool() const noexcept { return ec == std::errc{}; }
};

} // namespace decimal
} // namespace boost

#endif // !BOOST_DECIMAL_DISABLE_CLIB

#endif // BOOST_DECIMAL_DETAIL_FROM_CHARS_RESULT_HPP
