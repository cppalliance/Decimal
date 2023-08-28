// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mini_to_chars.hpp"
#include <boost/decimal/decimal32.hpp>
#include <boost/core/lightweight_test.hpp>
#include <iostream>
#include <sstream>

using namespace boost::decimal;

void test_istream()
{
    decimal32 val;
    std::stringstream in;
    in.str("1.234567e+06");
    in >> val;
    BOOST_TEST_EQ(val, decimal32(1234567, 0));

    errno = 0;
    decimal32 val2;
    std::stringstream in_zero;
    in_zero.str("0");
    in_zero >> val2;
    BOOST_TEST_EQ(val2, decimal32(0, 0)) && BOOST_TEST_EQ(errno, 0);

    decimal32 val3;
    std::stringstream bad;
    bad.str("");
    bad >> val3;
    BOOST_TEST_EQ(errno, EINVAL) && BOOST_TEST_NE(val3, std::numeric_limits<decimal32>::signaling_NaN());

    errno = 0;
    decimal32 inf_val;
    std::stringstream inf;
    inf.str("inf");
    inf >> inf_val;
    BOOST_TEST_EQ(inf_val, std::numeric_limits<decimal32>::infinity()) && BOOST_TEST_EQ(errno, 0);

    errno = 0;
    decimal32 inf_val2;
    std::stringstream inf2;
    inf2.str("INFINITY");
    inf2 >> inf_val2;
    BOOST_TEST_EQ(inf_val2, std::numeric_limits<decimal32>::infinity()) && BOOST_TEST_EQ(errno, 0);

    errno = 0;
    decimal32 snan_val;
    std::stringstream snan;
    snan.str("-nan(snan)");
    snan >> snan_val;
    BOOST_TEST_NE(snan_val, std::numeric_limits<decimal32>::signaling_NaN()) && BOOST_TEST_EQ(errno, 0);
}

void test_ostream()
{
    decimal32 val {1234567, 0};
    std::stringstream out;
    out << val;
    BOOST_TEST_CSTR_EQ(out.str().c_str(), "1.234567e+06");

    decimal32 zero {0, 0};
    std::stringstream zero_out;
    zero_out << zero;
    BOOST_TEST_CSTR_EQ(zero_out.str().c_str(), "0.0e+00");

    std::stringstream inf;
    inf << std::numeric_limits<decimal32>::infinity();
    BOOST_TEST_CSTR_EQ(inf.str().c_str(), "inf");

    std::stringstream qnan;
    qnan << std::numeric_limits<decimal32>::quiet_NaN();
    BOOST_TEST_CSTR_EQ(qnan.str().c_str(), "nan");

    std::stringstream snan;
    snan << std::numeric_limits<decimal32>::signaling_NaN();
    BOOST_TEST_CSTR_EQ(snan.str().c_str(), "nan(snan)");
}

int main()
{
    test_istream();
    test_ostream();

    return boost::report_errors();
}
