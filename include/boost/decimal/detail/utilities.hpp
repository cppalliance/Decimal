// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DETAIL_UTILITIES_HPP
#define BOOST_DECIMAL_DETAIL_UTILITIES_HPP

#include <boost/decimal/detail/config.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <cstddef>
#endif

namespace boost {
namespace decimal {
namespace detail {

template <typename T>
constexpr auto swap(T& x, T& y) noexcept -> void
{
    const T temp {x};
    x = y;
    y = temp;
}

template <typename T>
constexpr auto strlen(const T* str) noexcept -> std::size_t
{
    std::size_t i {};
    while (*str != '\0')
    {
        ++str;
        ++i;
    }

    return i;
}

} // namespace detail
} // namespace decimal
} // namespace boost

#endif //BOOST_DECIMAL_DETAIL_UTILITIES_HPP
