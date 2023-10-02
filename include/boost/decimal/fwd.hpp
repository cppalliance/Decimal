// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_FWD_HPP
#define BOOST_DECIMAL_FWD_HPP

#include <limits>

namespace boost { namespace decimal {

class decimal32;
class decimal64;

} // namespace decimal
} // namespace boost

namespace std {

template <>
#ifdef _MSC_VER
class numeric_limits<boost::decimal::decimal32>;
#else
struct numeric_limits<boost::decimal::decimal32>;
#endif

template <>
#ifdef _MSC_VER
class numeric_limits<boost::decimal::decimal64>;
#else
struct numeric_limits<boost::decimal::decimal64>;
#endif

} // Namespace std

#define BOOST_DECIMAL_HUGE_VAL_D32 std::numeric_limits<boost::decimal::decimal32>::infinity()
#define BOOST_DECIMAL_DEC_INFINITY std::numeric_limits<boost::decimal::decimal32>::infinity()
#define BOOST_DECIMAL_DEC_NAN std::numeric_limits<boost::decimal::decimal32>::signaling_NaN()

#endif // BOOST_DECIMAL_FWD_HPP
