// Copyright 2023 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "mini_to_chars.hpp"
#include <boost/decimal.hpp>
#include <boost/core/lightweight_test.hpp>
#include <iostream>
#include <sstream>
#include <random>
#include <cwchar>

using namespace boost::decimal;

static constexpr auto N {1024U};

template <typename T>
void roundtrip_strtod()
{
    std::mt19937_64 rng(42);

    std::uniform_int_distribution<std::int64_t> sig(1'000'000, 9'999'999);
    std::uniform_int_distribution<std::int32_t> exp(std::numeric_limits<T>::min_exponent10 + 19,
                                                    std::numeric_limits<T>::max_exponent10 - 19);

    for (std::size_t i {}; i < N; ++i)
    {
        const T val {sig(rng), exp(rng)};
        std::stringstream ss;
        ss << val;
        char* endptr {};

        const T return_val {boost::decimal::strtod(ss.str().c_str(), &endptr)};
        const auto len {std::strlen(ss.str().c_str())};
        const auto dist {endptr - ss.str().c_str()};

        if (!BOOST_TEST_EQ(val, return_val) && BOOST_TEST_EQ(len, dist))
        {
            std::cerr << "Val 1: " << val
                      << "\nVal 2: " << return_val
                      << "\nStrlen: " << len
                      << "\n  Dist: " << dist << std::endl;
        }
    }
}

template <typename T>
void roundtrip_wcstrtod()
{
    std::mt19937_64 rng(42);

    std::uniform_int_distribution<std::int64_t> sig(1'000'000, 9'999'999);
    std::uniform_int_distribution<std::int32_t> exp(std::numeric_limits<T>::min_exponent10 + 19,
                                                    std::numeric_limits<T>::max_exponent10 - 19);

    for (std::size_t i {}; i < N; ++i)
    {
        const T val {sig(rng), exp(rng)};
        std::wstringstream ss;
        ss << val;
        wchar_t* endptr {};

        const T return_val {boost::decimal::wcstod(ss.str().c_str(), &endptr)};
        const auto len {ss.str().size()};
        const auto dist {endptr - ss.str().c_str()};

        if (!BOOST_TEST_EQ(val, return_val) && BOOST_TEST_EQ(len, dist))
        {
            std::cerr << "Val 1: " << val
                      << "\nVal 2: " << return_val
                      << "\nStrlen: " << len
                      << "\n  Dist: " << dist << std::endl;
        }
    }
}

template <typename T>
void test_strtod_edges()
{
    errno = 0;
    BOOST_TEST(isnan(boost::decimal::strtod(nullptr, nullptr))) && BOOST_TEST_EQ(errno, EINVAL);

    errno = 0;
    BOOST_TEST(isnan(boost::decimal::wcstod(nullptr, nullptr))) && BOOST_TEST_EQ(errno, EINVAL);

    errno = 0;
    const char* snan_str = "nan(snan)";
    BOOST_TEST(isnan(boost::decimal::strtod(snan_str, nullptr))) && BOOST_TEST_EQ(errno, 0);

    errno = 0;
    const char* qnan_str = "nan";
    BOOST_TEST(isnan(boost::decimal::strtod(qnan_str, nullptr))) && BOOST_TEST_EQ(errno, 0);

    errno = 0;
    const char* inf_str = "inf";
    BOOST_TEST(isinf(boost::decimal::strtod(inf_str, nullptr))) && BOOST_TEST_EQ(errno, 0);

    errno = 0;
    const char* junk_str = "junk";
    BOOST_TEST(isnan(boost::decimal::strtod(junk_str, nullptr))) && BOOST_TEST_EQ(errno, EINVAL);
}

int main()
{
    roundtrip_strtod<decimal32>();
    roundtrip_wcstrtod<decimal32>();
    test_strtod_edges<decimal32>();

    return boost::report_errors();
}
