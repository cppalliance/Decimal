// Copyright 2024 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/decimal/decimal128.hpp>
#include <boost/decimal/detail/apply_sign.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/detail/integer_search_trees.hpp>
#include <boost/decimal/detail/attributes.hpp>
#include <boost/decimal/detail/concepts.hpp>
#include <boost/decimal/detail/add_impl.hpp>
#include <boost/decimal/detail/sub_impl.hpp>
#include <boost/decimal/detail/mul_impl.hpp>
#include <boost/decimal/detail/div_impl.hpp>
#include <boost/decimal/detail/emulated128.hpp>
#include <boost/decimal/detail/ryu/ryu_generic_128.hpp>
#include <limits>
#include <cstdint>

#ifndef BOOST_DECIMAL_DECIMAL128_FAST_HPP
#define BOOST_DECIMAL_DECIMAL128_FAST_HPP

namespace boost {
namespace decimal {

namespace detail {

BOOST_DECIMAL_CONSTEXPR_VARIABLE auto d128_fast_inf = std::numeric_limits<uint128>::max();
BOOST_DECIMAL_CONSTEXPR_VARIABLE auto d128_fast_qnan = std::numeric_limits<uint128>::max() - 1;
BOOST_DECIMAL_CONSTEXPR_VARIABLE auto d128_fast_snan = std::numeric_limits<uint128>::max() - 2;

struct decimal128_fast_components
{
    using sig_type = uint128;

    uint128 sig;
    std::int32_t exp;
    bool sign;
};

} // namespace detail

class decimal128_fast final
{
public:
    using significand_type = detail::uint128;
    using exponent_type = std::uint_fast32_t;

private:
    // Instead of having to encode and decode at every operation
    // we store the constituent pieces directly

    significand_type significand_ {};
    exponent_type exponent_ {};
    bool sign_ {};

    constexpr auto isneg() const noexcept -> bool
    {
        return sign_;
    }

    constexpr auto full_significand() const noexcept -> significand_type
    {
        return significand_;
    }

    constexpr auto unbiased_exponent() const noexcept -> exponent_type
    {
        return exponent_;
    }

    constexpr auto biased_exponent() const noexcept -> std::int32_t
    {
        return static_cast<std::int32_t>(exponent_) - detail::bias_v<decimal128>;
    }

    template <typename Decimal, typename TargetType>
    friend constexpr auto to_integral_128(Decimal val) noexcept
        BOOST_DECIMAL_REQUIRES_TWO_RETURN(detail::is_decimal_floating_point_v, Decimal, detail::is_integral_v, TargetType, TargetType);

    template <typename Decimal, typename TargetType>
    friend BOOST_DECIMAL_CXX20_CONSTEXPR auto to_float(Decimal val) noexcept
        BOOST_DECIMAL_REQUIRES_TWO_RETURN(detail::is_decimal_floating_point_v, Decimal, detail::is_floating_point_v, TargetType, TargetType);

    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE TargetType, BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal>
        friend constexpr auto to_decimal(Decimal val) noexcept -> TargetType;

    friend constexpr auto d128f_div_impl(decimal128_fast lhs, decimal128_fast rhs, decimal128_fast& q, decimal128_fast& r) noexcept -> void;

    // Equality template between any integer type and decimal128
    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal, BOOST_DECIMAL_INTEGRAL Integer>
    friend constexpr auto mixed_equality_impl(Decimal lhs, Integer rhs) noexcept
        -> std::enable_if_t<(detail::is_decimal_floating_point_v<Decimal> && detail::is_integral_v<Integer>), bool>;

    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal1, BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal2>
    friend constexpr auto mixed_decimal_equality_impl(Decimal1 lhs, Decimal2 rhs) noexcept
        -> std::enable_if_t<(detail::is_decimal_floating_point_v<Decimal1> &&
                             detail::is_decimal_floating_point_v<Decimal2>), bool>;

    // Template to compare operator< for any integer type and decimal128
    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal, BOOST_DECIMAL_INTEGRAL Integer>
    friend constexpr auto less_impl(Decimal lhs, Integer rhs) noexcept
        -> std::enable_if_t<(detail::is_decimal_floating_point_v<Decimal> && detail::is_integral_v<Integer>), bool>;

    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal1, BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal2>
    friend constexpr auto mixed_decimal_less_impl(Decimal1 lhs, Decimal2 rhs) noexcept
        -> std::enable_if_t<(detail::is_decimal_floating_point_v<Decimal1> &&
                             detail::is_decimal_floating_point_v<Decimal2>), bool>;

public:
    constexpr decimal128_fast() noexcept = default;

    #ifdef BOOST_DECIMAL_HAS_CONCEPTS
    template <BOOST_DECIMAL_INTEGRAL T1, BOOST_DECIMAL_INTEGRAL T2>
    #else
    template <typename T1, typename T2, std::enable_if_t<detail::is_integral_v<T1> && detail::is_integral_v<T2>, bool> = true>
    #endif
    constexpr decimal128_fast(T1 coeff, T2 exp, bool sign = false) noexcept;

    #ifdef BOOST_DECIMAL_HAS_CONCEPTS
    template <BOOST_DECIMAL_INTEGRAL Integer>
    #else
    template <typename Integer, std::enable_if_t<detail::is_integral_v<Integer>, bool> = true>
    #endif
    constexpr decimal128_fast(Integer val) noexcept;

    #ifdef BOOST_DECIMAL_HAS_CONCEPTS
    template <BOOST_DECIMAL_REAL Float>
    #else
    template <typename Float, std::enable_if_t<detail::is_floating_point_v<Float>, bool> = true>
    #endif
    explicit BOOST_DECIMAL_CXX20_CONSTEXPR decimal128_fast(Float val) noexcept;

    friend constexpr auto direct_init_d128(significand_type significand, exponent_type exponent, bool sign) noexcept -> decimal128_fast;

    // Classification functions
    friend constexpr auto signbit(decimal128_fast val) noexcept -> bool;
    friend constexpr auto isinf(decimal128_fast val) noexcept -> bool;
    friend constexpr auto isnan(decimal128_fast val) noexcept -> bool;
    friend constexpr auto issignaling(decimal128_fast val) noexcept -> bool;
    friend constexpr auto isnormal(decimal128_fast val) noexcept -> bool;

    // Comparison operators
    friend constexpr auto operator==(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;
    friend constexpr auto operator!=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;
    friend constexpr auto operator<(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;
    friend constexpr auto operator<=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;
    friend constexpr auto operator>(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;
    friend constexpr auto operator>=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool;

    // Mixed comparison operators
    template <typename Integer>
    friend constexpr auto operator==(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator==(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator!=(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator!=(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator<(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator<(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator<=(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator<=(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator>(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator>(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator>=(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    template <typename Integer>
    friend constexpr auto operator>=(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool);

    #ifdef BOOST_DECIMAL_HAS_SPACESHIP_OPERATOR
    friend constexpr auto operator<=>(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> std::partial_ordering;

    template <typename Integer>
    friend constexpr auto operator<=>(const decimal128_fast& lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, std::partial_ordering);

    template <typename Integer>
    friend constexpr auto operator<=>(Integer lhs, const decimal128_fast& rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, std::partial_ordering);
    #endif

    // Unary arithmetic operators
    friend constexpr auto operator+(decimal128_fast rhs) noexcept -> decimal128_fast;
    friend constexpr auto operator-(decimal128_fast rhs) noexcept -> decimal128_fast;

    // Binary arithmetic operators
    friend constexpr auto operator+(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast;
    friend constexpr auto operator-(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast;
    friend constexpr auto operator*(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast;
    friend constexpr auto operator/(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast;
    friend constexpr auto operator%(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast;

    // Mixed type binary arithmetic operators
    template <typename Integer>
    friend constexpr auto operator+(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast);

    template <typename Integer>
    friend constexpr auto operator+(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast);

    template <typename Integer>
    friend constexpr auto operator-(decimal128_fast lhs, Integer rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast);

    template <typename Integer>
    friend constexpr auto operator-(Integer lhs, decimal128_fast rhs) noexcept
        BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast);

    // Conversions
    explicit constexpr operator bool() const noexcept;
    explicit constexpr operator int() const noexcept;
    explicit constexpr operator unsigned() const noexcept;
    explicit constexpr operator long() const noexcept;
    explicit constexpr operator unsigned long() const noexcept;
    explicit constexpr operator long long() const noexcept;
    explicit constexpr operator unsigned long long() const noexcept;
    explicit constexpr operator std::int8_t() const noexcept;
    explicit constexpr operator std::uint8_t() const noexcept;
    explicit constexpr operator std::int16_t() const noexcept;
    explicit constexpr operator std::uint16_t() const noexcept;

    #ifdef BOOST_DECIMAL_HAS_INT128
    explicit constexpr operator detail::int128_t() const noexcept;
    explicit constexpr operator detail::uint128_t() const noexcept;
    #endif

    explicit BOOST_DECIMAL_CXX20_CONSTEXPR operator float() const noexcept;
    explicit BOOST_DECIMAL_CXX20_CONSTEXPR operator double() const noexcept;
    explicit BOOST_DECIMAL_CXX20_CONSTEXPR operator long double() const noexcept;

    #ifdef BOOST_DECIMAL_HAS_FLOAT16
    explicit constexpr operator std::float16_t() const noexcept;
    #endif
    #ifdef BOOST_DECIMAL_HAS_FLOAT32
    explicit constexpr operator std::float32_t() const noexcept;
    #endif
    #ifdef BOOST_DECIMAL_HAS_FLOAT64
    explicit constexpr operator std::float64_t() const noexcept;
    #endif
    #ifdef BOOST_DECIMAL_HAS_BRAINFLOAT16
    explicit constexpr operator std::bfloat16_t() const noexcept;
    #endif

    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal, std::enable_if_t<detail::is_decimal_floating_point_v<Decimal>, bool> = true>
    explicit constexpr operator Decimal() const noexcept;

    // <cmath> functions that are better as friends
    template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE T>
    friend constexpr auto frexp10(T num, int* expptr) noexcept -> typename T::significand_type;

    #if !defined(BOOST_DECIMAL_DISABLE_CLIB)

    // LCOV_EXCL_START
    // TODO(mborland): Fix with STL bindings and delete
    template <typename charT, typename traits>
    friend auto operator<<(std::basic_ostream<charT, traits>& os, const decimal128_fast& d) -> std::basic_ostream<charT, traits>&
    {
        if (d.sign_)
        {
            os << "-";
        }
        else
        {
            os << "+";
        }

        os << d.significand_ << "e";
        const auto biased_exp {d.biased_exponent()};
        if (biased_exp > 0)
        {
            os << '+';
        }
        os << biased_exp;

        return os;
    }
    // LCOV_EXCL_STOP

    #endif
};

#ifdef BOOST_DECIMAL_HAS_CONCEPTS
template <BOOST_DECIMAL_INTEGRAL T1, BOOST_DECIMAL_INTEGRAL T2>
#else
template <typename T1, typename T2, std::enable_if_t<detail::is_integral_v<T1> && detail::is_integral_v<T2>, bool>>
#endif
constexpr decimal128_fast::decimal128_fast(T1 coeff, T2 exp, bool sign) noexcept
{
    using Unsigned_Integer = detail::make_unsigned_t<T1>;

    const bool isneg {coeff < static_cast<T1>(0) || sign};
    sign_ = isneg;
    Unsigned_Integer unsigned_coeff {detail::make_positive_unsigned(coeff)};

    auto unsigned_coeff_digits {detail::num_digits(unsigned_coeff)};
    const bool reduced {unsigned_coeff_digits > detail::precision_v<decimal128>};

    // Strip digits
    if (unsigned_coeff_digits > detail::precision_v<decimal128> + 1)
    {
        const auto digits_to_remove {unsigned_coeff_digits - (detail::precision_v<decimal128> + 1)};

        #if defined(__GNUC__) && !defined(__clang__)
        #  pragma GCC diagnostic push
        #  pragma GCC diagnostic ignored "-Wconversion"
        #endif

        unsigned_coeff /= detail::pow10(static_cast<Unsigned_Integer>(digits_to_remove));

        #if defined(__GNUC__) && !defined(__clang__)
        #  pragma GCC diagnostic pop
        #endif

        exp += digits_to_remove;
        unsigned_coeff_digits -= digits_to_remove;
    }

    // Round as required
    if (reduced)
    {
        exp += static_cast<T2>(detail::fenv_round(unsigned_coeff, isneg));
    }

    significand_ = static_cast<significand_type>(unsigned_coeff);

    // Normalize the handling of zeros
    if (significand_ == detail::uint128{UINT64_C(0), UINT64_C(0)})
    {
        exp = 0;
    }

    const auto biased_exp {static_cast<exponent_type>(exp + detail::bias_v<decimal128>)};
    if (biased_exp > detail::max_biased_exp_v<decimal128>)
    {
        significand_ = detail::d128_fast_inf;
    }
    else
    {
        exponent_ = biased_exp;
    }
}

#ifdef BOOST_DECIMAL_HAS_CONCEPTS
template <BOOST_DECIMAL_INTEGRAL Integer>
#else
template <typename Integer, std::enable_if_t<detail::is_integral_v<Integer>, bool>>
#endif
constexpr decimal128_fast::decimal128_fast(Integer val) noexcept
{
    using ConversionType = std::conditional_t<std::is_same<Integer, bool>::value, std::int32_t, Integer>;
    *this = decimal128_fast{static_cast<ConversionType>(val), 0, false};
}

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wfloat-equal"
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

#ifdef BOOST_DECIMAL_HAS_CONCEPTS
template <BOOST_DECIMAL_REAL Float>
#else
template <typename Float, std::enable_if_t<detail::is_floating_point_v<Float>, bool>>
#endif
BOOST_DECIMAL_CXX20_CONSTEXPR decimal128_fast::decimal128_fast(Float val) noexcept
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (val != val)
    {
        significand_ = detail::d128_fast_qnan;
    }
    else if (val == std::numeric_limits<Float>::infinity() || val == -std::numeric_limits<Float>::infinity())
    {
        significand_ = detail::d128_fast_inf;
    }
    else
    #endif
    {
        const auto components {detail::ryu::floating_point_to_fd128(val)};
        *this = decimal128_fast {components.mantissa, components.exponent, components.sign};
    }
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#endif

constexpr auto direct_init_d128(decimal128_fast::significand_type significand, decimal128_fast::exponent_type exponent, bool sign) noexcept -> decimal128_fast
{
    decimal128_fast val {};
    val.significand_ = significand;
    val.exponent_ = exponent;
    val.sign_ = sign;

    return val;
}

constexpr auto signbit(decimal128_fast val) noexcept -> bool
{
    return val.sign_;
}

constexpr auto isinf(decimal128_fast val) noexcept -> bool
{
    return val.significand_ == detail::d128_fast_inf;
}

constexpr auto isnan(decimal128_fast val) noexcept -> bool
{
    return val.significand_ == detail::d128_fast_qnan ||
           val.significand_ == detail::d128_fast_snan;
}

constexpr auto issignaling(decimal128_fast val) noexcept -> bool
{
    return val.significand_ == detail::d128_fast_snan;
}

constexpr auto isnormal(decimal128_fast val) noexcept -> bool
{
    if (val.exponent_ <= static_cast<decimal128_fast::exponent_type>(detail::precision_v<decimal128> - 1))
    {
        return false;
    }

    return (val.significand_ != 0) && isfinite(val);
}

constexpr auto operator==(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs) || isnan(rhs))
    {
        return false;
    }
    #endif

    return equal_parts_impl(lhs.significand_, lhs.biased_exponent(), lhs.sign_,
                            rhs.significand_, rhs.biased_exponent(), rhs.sign_);
}

template <typename Integer>
constexpr auto operator==(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return mixed_equality_impl(lhs, rhs);
}

template <typename Integer>
constexpr auto operator==(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return mixed_equality_impl(rhs, lhs);
}

constexpr auto operator!=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
    return !(lhs == rhs);
}

template <typename Integer>
constexpr auto operator!=(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return !(lhs == rhs);
}

template <typename Integer>
constexpr auto operator!=(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return !(lhs == rhs);
}

constexpr auto operator<(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
#ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs) || isnan(rhs) ||
        (!lhs.isneg() && rhs.isneg()))
    {
        return false;
    }
    else if (lhs.isneg() && !rhs.isneg())
    {
        return true;
    }
    else if (isfinite(lhs) && isinf(rhs))
    {
        return !signbit(rhs);
    }
    else if (isinf(lhs) && isfinite(rhs))
    {
        return signbit(rhs);
    }
#else
    if (!lhs.isneg() && rhs.isneg())
    {
        return false;
    }
    else if (lhs.isneg() && !rhs.isneg())
    {
        return true;
    }
#endif

    return less_parts_impl(lhs.significand_, lhs.biased_exponent(), lhs.sign_,
                           rhs.significand_, rhs.biased_exponent(), rhs.sign_);
}

template <typename Integer>
constexpr auto operator<(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return less_impl(lhs, rhs);
}

template <typename Integer>
constexpr auto operator<(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(rhs))
    {
        return false;
    }
    #endif

    return !less_impl(rhs, lhs) && lhs != rhs;
}

constexpr auto operator<=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs) || isnan(rhs))
    {
        return false;
    }
    #endif

    return !(rhs < lhs);
}

template <typename Integer>
constexpr auto operator<=(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs))
    {
        return false;
    }
    #endif

    return !(rhs < lhs);
}

template <typename Integer>
constexpr auto operator<=(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(rhs))
    {
        return false;
    }
    #endif

    return !(rhs < lhs);
}

constexpr auto operator>(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
    return rhs < lhs;
}

template <typename Integer>
constexpr auto operator>(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return rhs < lhs;
}

template <typename Integer>
constexpr auto operator>(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    return rhs < lhs;
}

constexpr auto operator>=(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> bool
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs) || isnan(rhs))
    {
        return false;
    }
    #endif

    return !(lhs < rhs);
}

template <typename Integer>
constexpr auto operator>=(decimal128_fast lhs, Integer rhs) noexcept
BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs))
    {
        return false;
    }
    #endif

    return !(lhs < rhs);
}

template <typename Integer>
constexpr auto operator>=(Integer lhs, decimal128_fast rhs) noexcept
BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, bool)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(rhs))
    {
        return false;
    }
    #endif

    return !(lhs < rhs);
}

#ifdef BOOST_DECIMAL_HAS_SPACESHIP_OPERATOR

constexpr auto operator<=>(const decimal128_fast& lhs, const decimal128_fast& rhs) noexcept -> std::partial_ordering
{
    if (lhs < rhs)
    {
        return std::partial_ordering::less;
    }
    else if (lhs > rhs)
    {
        return std::partial_ordering::greater;
    }
    else if (lhs == rhs)
    {
        return std::partial_ordering::equivalent;
    }

    return std::partial_ordering::unordered;
}

template <typename Integer>
constexpr auto operator<=>(const decimal128_fast& lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, std::partial_ordering)
{
    if (lhs < rhs)
    {
        return std::partial_ordering::less;
    }
    else if (lhs > rhs)
    {
        return std::partial_ordering::greater;
    }
    else if (lhs == rhs)
    {
        return std::partial_ordering::equivalent;
    }

    return std::partial_ordering::unordered;
}

template <typename Integer>
constexpr auto operator<=>(Integer lhs, const decimal128_fast& rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, std::partial_ordering)
{
    if (lhs < rhs)
    {
        return std::partial_ordering::less;
    }
    else if (lhs > rhs)
    {
        return std::partial_ordering::greater;
    }
    else if (lhs == rhs)
    {
        return std::partial_ordering::equivalent;
    }

    return std::partial_ordering::unordered;
}

#endif

constexpr auto operator+(decimal128_fast rhs) noexcept -> decimal128_fast
{
    return rhs;
}

constexpr auto operator-(decimal128_fast rhs) noexcept -> decimal128_fast
{
    rhs.sign_ = !rhs.sign_;
    return rhs;
}

constexpr auto operator+(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    constexpr decimal128_fast zero {0, 0};

    const auto res {detail::check_non_finite(lhs, rhs)};
    if (res != zero)
    {
        return res;
    }
    #endif

    bool lhs_bigger {lhs > rhs};
    if (lhs.isneg() && rhs.isneg())
    {
        lhs_bigger = !lhs_bigger;
    }

    // Ensure that lhs is always the larger for ease of impl
    if (!lhs_bigger)
    {
        detail::swap(lhs, rhs);
    }

    if (!lhs.isneg() && rhs.isneg())
    {
        return lhs - abs(rhs);
    }

    auto lhs_sig {lhs.full_significand()};
    auto lhs_exp {lhs.biased_exponent()};
    detail::normalize<decimal64>(lhs_sig, lhs_exp);

    auto rhs_sig {rhs.full_significand()};
    auto rhs_exp {rhs.biased_exponent()};
    detail::normalize<decimal64>(rhs_sig, rhs_exp);

    const auto result {detail::d128_add_impl<detail::decimal128_fast_components>(
            lhs.significand_, lhs.biased_exponent(), lhs.sign_,
            rhs.significand_, rhs.biased_exponent(), rhs.sign_)};

    return {result.sig, result.exp, result.sign};
};

template <typename Integer>
constexpr auto operator+(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isnan(lhs) || isinf(lhs))
    {
        return lhs;
    }
    #endif

    bool lhs_bigger {lhs > rhs};
    if (lhs.isneg() && (rhs < 0))
    {
        lhs_bigger = !lhs_bigger;
    }
    bool abs_lhs_bigger {abs(lhs) > detail::make_positive_unsigned(rhs)};

    auto sig_lhs {lhs.full_significand()};
    auto exp_lhs {lhs.biased_exponent()};
    detail::normalize<decimal128>(sig_lhs, exp_lhs);
    auto lhs_components {detail::decimal128_fast_components{sig_lhs, exp_lhs, lhs.isneg()}};

    auto sig_rhs {static_cast<detail::uint128>(detail::make_positive_unsigned(rhs))};
    std::int32_t exp_rhs {0};
    detail::normalize<decimal128>(sig_rhs, exp_rhs);
    auto unsigned_sig_rhs = detail::make_positive_unsigned(sig_rhs);
    auto rhs_components {detail::decimal128_fast_components{unsigned_sig_rhs, exp_rhs, (rhs < 0)}};

    if (!lhs_bigger)
    {
        detail::swap(lhs_components, rhs_components);
        lhs_bigger = !lhs_bigger;
        abs_lhs_bigger = !abs_lhs_bigger;
    }

    detail::decimal128_fast_components result {};

    #ifdef BOOST_DECIMAL_DEBUG_ADD
    std::cerr << "Lhs sig: " << lhs_components.sig
              << "\nLhs exp: " << lhs_components.exp
              << "\nRhs sig: " << rhs_components.sig
              << "\nRhs exp: " << rhs_components.exp << std::endl;
    #endif

    if (!lhs_components.sign && rhs_components.sign)
    {
        result = detail::d128_sub_impl<detail::decimal128_fast_components>(
                lhs_components.sig, lhs_components.exp, lhs_components.sign,
                rhs_components.sig, rhs_components.exp, rhs_components.sign,
                abs_lhs_bigger);
    }
    else
    {
        result = detail::d128_add_impl<detail::decimal128_fast_components>(
                lhs_components.sig, lhs_components.exp, lhs_components.sign,
                rhs_components.sig, rhs_components.exp, rhs_components.sign);
    }

    return {result.sig, result.exp, result.sign};
}

template <typename Integer>
constexpr auto operator+(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast)
{
    return rhs + lhs;
}

constexpr auto operator-(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    constexpr decimal128_fast zero {0, 0};

    const auto res {detail::check_non_finite(lhs, rhs)};
    if (res != zero)
    {
        return res;
    }
    #endif

    if (!lhs.isneg() && rhs.isneg())
    {
        return lhs + (-rhs);
    }

    const bool abs_lhs_bigger {abs(lhs) > abs(rhs)};

    auto sig_lhs {lhs.full_significand()};
    auto exp_lhs {lhs.biased_exponent()};
    detail::normalize<decimal64>(sig_lhs, exp_lhs);

    auto sig_rhs {rhs.full_significand()};
    auto exp_rhs {rhs.biased_exponent()};
    detail::normalize<decimal64>(sig_rhs, exp_rhs);

    const auto result {detail::d128_sub_impl<detail::decimal128_fast_components>(
            lhs.significand_, lhs.biased_exponent(), lhs.sign_,
            rhs.significand_, rhs.biased_exponent(), rhs.sign_,
            abs_lhs_bigger
    )};

    return {result.sig, result.exp, result.sign};
}

template <typename Integer>
constexpr auto operator-(decimal128_fast lhs, Integer rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isinf(lhs) || isnan(lhs))
    {
        return lhs;
    }
    #endif

    if (!lhs.isneg() && (rhs < 0))
    {
        return lhs + detail::make_positive_unsigned(rhs);
    }

    const bool abs_lhs_bigger {abs(lhs) > detail::make_positive_unsigned(rhs)};

    auto sig_lhs {lhs.full_significand()};
    auto exp_lhs {lhs.biased_exponent()};
    detail::normalize<decimal128>(sig_lhs, exp_lhs);
    auto lhs_components {detail::decimal128_fast_components{sig_lhs, exp_lhs, lhs.isneg()}};

    auto sig_rhs {static_cast<detail::uint128>(detail::make_positive_unsigned(rhs))};
    std::int32_t exp_rhs {0};
    detail::normalize<decimal128>(sig_rhs, exp_rhs);
    auto unsigned_sig_rhs {detail::make_positive_unsigned(sig_rhs)};
    auto rhs_components {detail::decimal128_fast_components{unsigned_sig_rhs, exp_rhs, (rhs < 0)}};

    const auto result {detail::d128_sub_impl<detail::decimal128_fast_components>(
            lhs_components.sig, lhs_components.exp, lhs_components.sign,
            rhs_components.sig, rhs_components.exp, rhs_components.sign,
            abs_lhs_bigger)};

    return {result.sig, result.exp, result.sign};
}

template <typename Integer>
constexpr auto operator-(Integer lhs, decimal128_fast rhs) noexcept
    BOOST_DECIMAL_REQUIRES_RETURN(detail::is_integral_v, Integer, decimal128_fast)
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    if (isinf(rhs) || isnan(rhs))
    {
        return rhs;
    }
    #endif

    if (lhs >= 0 && rhs.isneg())
    {
        return lhs + (-rhs);
    }

    const bool abs_lhs_bigger {detail::make_positive_unsigned(lhs) > abs(rhs)};

    auto sig_lhs {static_cast<detail::uint128>(detail::make_positive_unsigned(lhs))};
    std::int32_t exp_lhs {0};
    detail::normalize<decimal128>(sig_lhs, exp_lhs);
    auto unsigned_sig_lhs {detail::make_positive_unsigned(sig_lhs)};
    auto lhs_components {detail::decimal128_fast_components{unsigned_sig_lhs, exp_lhs, (lhs < 0)}};

    auto sig_rhs {rhs.full_significand()};
    auto exp_rhs {rhs.biased_exponent()};
    detail::normalize<decimal128>(sig_rhs, exp_rhs);
    auto rhs_components {detail::decimal128_fast_components{sig_rhs, exp_rhs, rhs.isneg()}};

    const auto result {detail::d128_sub_impl<detail::decimal128_fast_components>(
            lhs_components.sig, lhs_components.exp, lhs_components.sign,
            rhs_components.sig, rhs_components.exp, rhs_components.sign,
            abs_lhs_bigger)};

    return {result.sig, result.exp, result.sign};
}

constexpr auto operator*(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    constexpr decimal128_fast zero {0, 0};

    const auto non_finite {detail::check_non_finite(lhs, rhs)};
    if (non_finite != zero)
    {
        return non_finite;
    }
    #endif

    auto lhs_sig {lhs.full_significand()};
    auto lhs_exp {lhs.biased_exponent()};
    const auto lhs_zeros {detail::remove_trailing_zeros(lhs_sig)};
    lhs_sig = lhs_zeros.trimmed_number;
    lhs_exp += static_cast<std::int32_t>(lhs_zeros.number_of_removed_zeros);

    auto rhs_sig {rhs.full_significand()};
    auto rhs_exp {rhs.biased_exponent()};
    const auto rhs_zeros {detail::remove_trailing_zeros(rhs_sig)};
    rhs_sig = rhs_zeros.trimmed_number;
    rhs_exp += static_cast<std::int32_t>(rhs_zeros.number_of_removed_zeros);

    const auto result {detail::d128_mul_impl<detail::decimal128_components>(
            lhs_sig, lhs_exp, lhs.isneg(),
            rhs_sig, rhs_exp, rhs.isneg())};

    return {result.sig, result.exp, result.sign};
}

constexpr auto d128f_div_impl(decimal128_fast lhs, decimal128_fast rhs, decimal128_fast& q, decimal128_fast& r) noexcept -> void
{
    #ifndef BOOST_DECIMAL_FAST_MATH
    // Check pre-conditions
    constexpr decimal128_fast zero {0, 0};
    constexpr decimal128_fast nan {boost::decimal::direct_init_d128(boost::decimal::detail::d128_fast_qnan, 0, false)};
    constexpr decimal128_fast inf {boost::decimal::direct_init_d128(boost::decimal::detail::d128_fast_inf, 0, false)};

    const bool sign {lhs.isneg() != rhs.isneg()};

    const auto lhs_fp {fpclassify(lhs)};
    const auto rhs_fp {fpclassify(rhs)};

    if (lhs_fp == FP_NAN || rhs_fp == FP_NAN)
    {
        q = nan;
        r = nan;
        return;
    }

    switch (lhs_fp)
    {
        case FP_INFINITE:
            q = sign ? -inf : inf;
            r = zero;
            return;
        case FP_ZERO:
            q = sign ? -zero : zero;
            r = sign ? -zero : zero;
            return;
        default:
            static_cast<void>(lhs);
    }

    switch (rhs_fp)
    {
        case FP_ZERO:
            q = inf;
            r = zero;
            return;
        case FP_INFINITE:
            q = sign ? -zero : zero;
            r = lhs;
            return;
        default:
            static_cast<void>(rhs);
    }
    #else
    static_cast<void>(r);
    #endif

    auto sig_lhs {lhs.full_significand()};
    auto exp_lhs {lhs.biased_exponent()};
    detail::normalize<decimal128>(sig_lhs, exp_lhs);

    auto sig_rhs {rhs.full_significand()};
    auto exp_rhs {rhs.biased_exponent()};
    detail::normalize<decimal128>(sig_rhs, exp_rhs);

    #ifdef BOOST_DECIMAL_DEBUG
    std::cerr << "sig lhs: " << sig_lhs
              << "\nexp lhs: " << exp_lhs
              << "\nsig rhs: " << sig_rhs
              << "\nexp rhs: " << exp_rhs << std::endl;
    #endif

    detail::decimal128_fast_components lhs_components {sig_lhs, exp_lhs, lhs.isneg()};
    detail::decimal128_fast_components rhs_components {sig_rhs, exp_rhs, rhs.isneg()};
    detail::decimal128_fast_components q_components {};

    detail::d128_generic_div_impl(lhs_components, rhs_components, q_components);

    q = decimal128_fast(q_components.sig, q_components.exp, q_components.sign);
}

constexpr auto d128f_mod_impl(decimal128_fast lhs, decimal128_fast rhs, const decimal128_fast& q, decimal128_fast& r) -> void
{
    constexpr decimal128_fast zero {0, 0};

    auto q_trunc {q > zero ? floor(q) : ceil(q)};
    r = lhs - (decimal128_fast(q_trunc) * rhs);
};

constexpr auto operator/(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast
{
    decimal128_fast q {};
    decimal128_fast r {};
    d128f_div_impl(lhs, rhs, q, r);

    return q;
};

constexpr auto operator%(decimal128_fast lhs, decimal128_fast rhs) noexcept -> decimal128_fast
{
    decimal128_fast q {};
    decimal128_fast r {};
    d128f_div_impl(lhs, rhs, q, r);
    d128f_mod_impl(lhs, rhs, q, r);

    return r;
};

constexpr decimal128_fast::operator bool() const noexcept
{
    constexpr decimal128_fast zero {0, 0};
    return *this != zero;
}

constexpr decimal128_fast::operator int() const noexcept
{
    return to_integral_128<decimal128_fast, int>(*this);
}

constexpr decimal128_fast::operator unsigned() const noexcept
{
    return to_integral_128<decimal128_fast, unsigned>(*this);
}

constexpr decimal128_fast::operator long() const noexcept
{
    return to_integral_128<decimal128_fast, long>(*this);
}

constexpr decimal128_fast::operator unsigned long() const noexcept
{
    return to_integral_128<decimal128_fast, unsigned long>(*this);
}

constexpr decimal128_fast::operator long long() const noexcept
{
    return to_integral_128<decimal128_fast, long long>(*this);
}

constexpr decimal128_fast::operator unsigned long long() const noexcept
{
    return to_integral_128<decimal128_fast, unsigned long long>(*this);
}

constexpr decimal128_fast::operator std::int8_t() const noexcept
{
    return to_integral_128<decimal128_fast, std::int8_t>(*this);
}

constexpr decimal128_fast::operator std::uint8_t() const noexcept
{
    return to_integral_128<decimal128_fast, std::uint8_t>(*this);
}

constexpr decimal128_fast::operator std::int16_t() const noexcept
{
    return to_integral_128<decimal128_fast, std::int16_t>(*this);
}

constexpr decimal128_fast::operator std::uint16_t() const noexcept
{
    return to_integral_128<decimal128_fast, std::uint16_t>(*this);
}

#ifdef BOOST_DECIMAL_HAS_INT128

constexpr decimal128_fast::operator detail::int128_t() const noexcept
{
    return to_integral_128<decimal128_fast, detail::int128_t>(*this);
}

constexpr decimal128_fast::operator detail::uint128_t() const noexcept
{
    return to_integral_128<decimal128_fast, detail::uint128_t>(*this);
}

#endif // BOOST_DECIMAL_HAS_INT128

BOOST_DECIMAL_CXX20_CONSTEXPR decimal128_fast::operator float() const noexcept
{
    return to_float<decimal128_fast, float>(*this);
}

BOOST_DECIMAL_CXX20_CONSTEXPR decimal128_fast::operator double() const noexcept
{
    return to_float<decimal128_fast, double>(*this);
}

BOOST_DECIMAL_CXX20_CONSTEXPR decimal128_fast::operator long double() const noexcept
{
    return to_float<decimal128_fast, long double>(*this);
}

#ifdef BOOST_DECIMAL_HAS_FLOAT16
constexpr decimal128_fast::operator std::float16_t() const noexcept
{
    return static_cast<std::float16_t>(to_float<decimal128_fast, float>(*this));
}
#endif
#ifdef BOOST_DECIMAL_HAS_FLOAT32
constexpr decimal128_fast::operator std::float32_t() const noexcept
{
    return static_cast<std::float32_t>(to_float<decimal128_fast, float>(*this));
}
#endif
#ifdef BOOST_DECIMAL_HAS_FLOAT64
constexpr decimal128_fast::operator std::float64_t() const noexcept
{
    return static_cast<std::float64_t>(to_float<decimal128_fast, double>(*this));
}
#endif
#ifdef BOOST_DECIMAL_HAS_BRAINFLOAT16
constexpr decimal128_fast::operator std::bfloat16_t() const noexcept
{
    return static_cast<std::bfloat16_t>(to_float<decimal128_fast, float>(*this));
}
#endif

template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE Decimal, std::enable_if_t<detail::is_decimal_floating_point_v<Decimal>, bool>>
constexpr decimal128_fast::operator Decimal() const noexcept
{
    return to_decimal<Decimal>(*this);
}

} // namespace decimal
} // namespace boost

namespace std {

BOOST_DECIMAL_EXPORT template<>
#ifdef _MSC_VER
class numeric_limits<boost::decimal::decimal128_fast>
#else
struct numeric_limits<boost::decimal::decimal128_fast>
#endif
{

#ifdef _MSC_VER
    public:
#endif

    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_specialized = true;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_signed = true;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_integer = false;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_exact = false;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool has_infinity = true;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool has_quiet_NaN = true;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool has_signaling_NaN = true;

    // These members were deprecated in C++23
    #if ((!defined(_MSC_VER) && (__cplusplus <= 202002L)) || (defined(_MSC_VER) && (_MSVC_LANG <= 202002L)))
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr std::float_denorm_style has_denorm = std::denorm_present;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool has_denorm_loss = true;
    #endif

    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr std::float_round_style round_style = std::round_indeterminate;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_iec559 = false;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_bounded = true;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool is_modulo = false;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  digits = 34;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  digits10 = digits;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  max_digits10 = digits;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  radix = 10;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  min_exponent = -6142;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  min_exponent10 = min_exponent;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  max_exponent = 6145;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr int  max_exponent10 = max_exponent;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool traps = numeric_limits<std::uint64_t>::traps;
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr bool tinyness_before = true;

    // Member functions
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto (min)        () -> boost::decimal::decimal128_fast { return {1, min_exponent}; }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto (max)        () -> boost::decimal::decimal128_fast { return {boost::decimal::detail::uint128{UINT64_C(999'999'999'999'999), UINT64_C(9'999'999'999'999'999'999)}, max_exponent}; }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto lowest       () -> boost::decimal::decimal128_fast { return {boost::decimal::detail::uint128{UINT64_C(999'999'999'999'999), UINT64_C(9'999'999'999'999'999'999)}, max_exponent, true}; }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto epsilon      () -> boost::decimal::decimal128_fast { return {1, -34}; }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto round_error  () -> boost::decimal::decimal128_fast { return epsilon(); }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto infinity     () -> boost::decimal::decimal128_fast { return boost::decimal::direct_init_d128(boost::decimal::detail::d128_fast_inf, 0, false); }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto quiet_NaN    () -> boost::decimal::decimal128_fast { return boost::decimal::direct_init_d128(boost::decimal::detail::d128_fast_qnan, 0, false); }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto signaling_NaN() -> boost::decimal::decimal128_fast { return boost::decimal::direct_init_d128(boost::decimal::detail::d128_fast_snan, 0, false); }
    BOOST_DECIMAL_ATTRIBUTE_UNUSED static constexpr auto denorm_min   () -> boost::decimal::decimal128_fast { return {1, boost::decimal::detail::etiny_v<boost::decimal::decimal128>}; }
};

}

#endif //BOOST_DECIMAL_DECIMAL128_FAST_HPP
