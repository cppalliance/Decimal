// Copyright 2024 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_DECIMAL_DPD_CONVERSION_HPP
#define BOOST_DECIMAL_DPD_CONVERSION_HPP

#include <boost/decimal/bid_conversion.hpp>
#include <boost/decimal/detail/config.hpp>
#include <boost/decimal/detail/concepts.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <cstdint>
#endif

namespace boost {
namespace decimal {

namespace detail {

// See IEEE 754-2008 tables 3-3 and 3-4 for explanation
BOOST_DECIMAL_CONSTEXPR_VARIABLE std::uint16_t dpd_table[1000] =
{
    0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007,
    0x002, 0x003, 0x004, 0x005, 0x006, 0x007, 0x004, 0x005,
    0x006, 0x007, 0x006, 0x007, 0x008, 0x009, 0x00a, 0x00b,
    0x00c, 0x00d, 0x00e, 0x00f, 0x00a, 0x00b, 0x00c, 0x00d,
    0x00e, 0x00f, 0x00c, 0x00d, 0x00e, 0x00f, 0x00e, 0x00f,
    0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017,
    0x012, 0x013, 0x014, 0x015, 0x016, 0x017, 0x014, 0x015,
    0x016, 0x017, 0x016, 0x017, 0x018, 0x019, 0x01a, 0x01b,
    0x01c, 0x01d, 0x01e, 0x01f, 0x01a, 0x01b, 0x01c, 0x01d,
    0x01e, 0x01f, 0x01c, 0x01d, 0x01e, 0x01f, 0x01e, 0x01f,
    0x02c, 0x02d, 0x02e, 0x02f, 0x02c, 0x02d, 0x02e, 0x02f,
    0x02b, 0x02f, 0x03c, 0x03d, 0x03e, 0x03f, 0x03c, 0x03d,
    0x03e, 0x03f, 0x03b, 0x03f, 0x040, 0x041, 0x042, 0x043,
    0x044, 0x045, 0x046, 0x047, 0x042, 0x043, 0x044, 0x045,
    0x046, 0x047, 0x044, 0x045, 0x046, 0x047, 0x046, 0x047,
    0x048, 0x049, 0x04a, 0x04b, 0x04c, 0x04d, 0x04e, 0x04f,
    0x04a, 0x04b, 0x04c, 0x04d, 0x04e, 0x04f, 0x04c, 0x04d,
    0x04e, 0x04f, 0x04e, 0x04f, 0x050, 0x051, 0x052, 0x053,
    0x054, 0x055, 0x056, 0x057, 0x052, 0x053, 0x054, 0x055,
    0x056, 0x057, 0x054, 0x055, 0x056, 0x057, 0x056, 0x057,
    0x058, 0x059, 0x05a, 0x05b, 0x05c, 0x05d, 0x05e, 0x05f,
    0x05a, 0x05b, 0x05c, 0x05d, 0x05e, 0x05f, 0x05c, 0x05d,
    0x05e, 0x05f, 0x05e, 0x05f, 0x06c, 0x06d, 0x06e, 0x06f,
    0x06c, 0x06d, 0x06e, 0x06f, 0x06b, 0x06f, 0x07c, 0x07d,
    0x07e, 0x07f, 0x07c, 0x07d, 0x07e, 0x07f, 0x07b, 0x07f,
    0x080, 0x081, 0x082, 0x083, 0x084, 0x085, 0x086, 0x087,
    0x082, 0x083, 0x084, 0x085, 0x086, 0x087, 0x084, 0x085,
    0x086, 0x087, 0x086, 0x087, 0x088, 0x089, 0x08a, 0x08b,
    0x08c, 0x08d, 0x08e, 0x08f, 0x08a, 0x08b, 0x08c, 0x08d,
    0x08e, 0x08f, 0x08c, 0x08d, 0x08e, 0x08f, 0x08e, 0x08f,
    0x090, 0x091, 0x092, 0x093, 0x094, 0x095, 0x096, 0x097,
    0x092, 0x093, 0x094, 0x095, 0x096, 0x097, 0x094, 0x095,
    0x096, 0x097, 0x096, 0x097, 0x098, 0x099, 0x09a, 0x09b,
    0x09c, 0x09d, 0x09e, 0x09f, 0x09a, 0x09b, 0x09c, 0x09d,
    0x09e, 0x09f, 0x09c, 0x09d, 0x09e, 0x09f, 0x09e, 0x09f,
    0x0ac, 0x0ad, 0x0ae, 0x0af, 0x0ac, 0x0ad, 0x0ae, 0x0af,
    0x0ab, 0x0af, 0x0bc, 0x0bd, 0x0be, 0x0bf, 0x0bc, 0x0bd,
    0x0be, 0x0bf, 0x0bb, 0x0bf, 0x0c0, 0x0c1, 0x0c2, 0x0c3,
    0x0c4, 0x0c5, 0x0c6, 0x0c7, 0x0c2, 0x0c3, 0x0c4, 0x0c5,
    0x0c6, 0x0c7, 0x0c4, 0x0c5, 0x0c6, 0x0c7, 0x0c6, 0x0c7,
    0x0c8, 0x0c9, 0x0ca, 0x0cb, 0x0cc, 0x0cd, 0x0ce, 0x0cf,
    0x0ca, 0x0cb, 0x0cc, 0x0cd, 0x0ce, 0x0cf, 0x0cc, 0x0cd,
    0x0ce, 0x0cf, 0x0ce, 0x0cf, 0x0d0, 0x0d1, 0x0d2, 0x0d3,
    0x0d4, 0x0d5, 0x0d6, 0x0d7, 0x0d2, 0x0d3, 0x0d4, 0x0d5,
    0x0d6, 0x0d7, 0x0d4, 0x0d5, 0x0d6, 0x0d7, 0x0d6, 0x0d7,
    0x0d8, 0x0d9, 0x0da, 0x0db, 0x0dc, 0x0dd, 0x0de, 0x0df,
    0x0da, 0x0db, 0x0dc, 0x0dd, 0x0de, 0x0df, 0x0dc, 0x0dd,
    0x0de, 0x0df, 0x0de, 0x0df, 0x0ec, 0x0ed, 0x0ee, 0x0ef,
    0x0ec, 0x0ed, 0x0ee, 0x0ef, 0x0eb, 0x0ef, 0x0fc, 0x0fd,
    0x0fe, 0x0ff, 0x0fc, 0x0fd, 0x0fe, 0x0ff, 0x0fb, 0x0ff,
    0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107,
    0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x104, 0x105,
    0x106, 0x107, 0x106, 0x107, 0x108, 0x109, 0x10a, 0x10b,
    0x10c, 0x10d, 0x10e, 0x10f, 0x10a, 0x10b, 0x10c, 0x10d,
    0x10e, 0x10f, 0x10c, 0x10d, 0x10e, 0x10f, 0x10e, 0x10f,
    0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117,
    0x112, 0x113, 0x114, 0x115, 0x116, 0x117, 0x114, 0x115,
    0x116, 0x117, 0x116, 0x117, 0x118, 0x119, 0x11a, 0x11b,
    0x11c, 0x11d, 0x11e, 0x11f, 0x11a, 0x11b, 0x11c, 0x11d,
    0x11e, 0x11f, 0x11c, 0x11d, 0x11e, 0x11f, 0x11e, 0x11f,
    0x12c, 0x12d, 0x12e, 0x12f, 0x12c, 0x12d, 0x12e, 0x12f,
    0x12b, 0x12f, 0x13c, 0x13d, 0x13e, 0x13f, 0x13c, 0x13d,
    0x13e, 0x13f, 0x13b, 0x13f, 0x140, 0x141, 0x142, 0x143,
    0x144, 0x145, 0x146, 0x147, 0x142, 0x143, 0x144, 0x145,
    0x146, 0x147, 0x144, 0x145, 0x146, 0x147, 0x146, 0x147,
    0x148, 0x149, 0x14a, 0x14b, 0x14c, 0x14d, 0x14e, 0x14f,
    0x14a, 0x14b, 0x14c, 0x14d, 0x14e, 0x14f, 0x14c, 0x14d,
    0x14e, 0x14f, 0x14e, 0x14f, 0x150, 0x151, 0x152, 0x153,
    0x154, 0x155, 0x156, 0x157, 0x152, 0x153, 0x154, 0x155,
    0x156, 0x157, 0x154, 0x155, 0x156, 0x157, 0x156, 0x157,
    0x158, 0x159, 0x15a, 0x15b, 0x15c, 0x15d, 0x15e, 0x15f,
    0x15a, 0x15b, 0x15c, 0x15d, 0x15e, 0x15f, 0x15c, 0x15d,
    0x15e, 0x15f, 0x15e, 0x15f, 0x16c, 0x16d, 0x16e, 0x16f,
    0x16c, 0x16d, 0x16e, 0x16f, 0x16b, 0x16f, 0x17c, 0x17d,
    0x17e, 0x17f, 0x17c, 0x17d, 0x17e, 0x17f, 0x17b, 0x17f,
    0x180, 0x181, 0x182, 0x183, 0x184, 0x185, 0x186, 0x187,
    0x182, 0x183, 0x184, 0x185, 0x186, 0x187, 0x184, 0x185,
    0x186, 0x187, 0x186, 0x187, 0x188, 0x189, 0x18a, 0x18b,
    0x18c, 0x18d, 0x18e, 0x18f, 0x18a, 0x18b, 0x18c, 0x18d,
    0x18e, 0x18f, 0x18c, 0x18d, 0x18e, 0x18f, 0x18e, 0x18f,
    0x190, 0x191, 0x192, 0x193, 0x194, 0x195, 0x196, 0x197,
    0x192, 0x193, 0x194, 0x195, 0x196, 0x197, 0x194, 0x195,
    0x196, 0x197, 0x196, 0x197, 0x198, 0x199, 0x19a, 0x19b,
    0x19c, 0x19d, 0x19e, 0x19f, 0x19a, 0x19b, 0x19c, 0x19d,
    0x19e, 0x19f, 0x19c, 0x19d, 0x19e, 0x19f, 0x19e, 0x19f,
    0x1ac, 0x1ad, 0x1ae, 0x1af, 0x1ac, 0x1ad, 0x1ae, 0x1af,
    0x1ab, 0x1af, 0x1bc, 0x1bd, 0x1be, 0x1bf, 0x1bc, 0x1bd,
    0x1be, 0x1bf, 0x1bb, 0x1bf, 0x1c0, 0x1c1, 0x1c2, 0x1c3,
    0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1c2, 0x1c3, 0x1c4, 0x1c5,
    0x1c6, 0x1c7, 0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1c6, 0x1c7,
    0x1c8, 0x1c9, 0x1ca, 0x1cb, 0x1cc, 0x1cd, 0x1ce, 0x1cf,
    0x1ca, 0x1cb, 0x1cc, 0x1cd, 0x1ce, 0x1cf, 0x1cc, 0x1cd,
    0x1ce, 0x1cf, 0x1ce, 0x1cf, 0x1d0, 0x1d1, 0x1d2, 0x1d3,
    0x1d4, 0x1d5, 0x1d6, 0x1d7, 0x1d2, 0x1d3, 0x1d4, 0x1d5,
    0x1d6, 0x1d7, 0x1d4, 0x1d5, 0x1d6, 0x1d7, 0x1d6, 0x1d7,
    0x1d8, 0x1d9, 0x1da, 0x1db, 0x1dc, 0x1dd, 0x1de, 0x1df,
    0x1da, 0x1db, 0x1dc, 0x1dd, 0x1de, 0x1df, 0x1dc, 0x1dd,
    0x1de, 0x1df, 0x1de, 0x1df, 0x1ec, 0x1ed, 0x1ee, 0x1ef,
    0x1ec, 0x1ed, 0x1ee, 0x1ef, 0x1eb, 0x1ef, 0x1fc, 0x1fd,
    0x1fe, 0x1ff, 0x1fc, 0x1fd, 0x1fe, 0x1ff, 0x1fb, 0x1ff,
    0x2c0, 0x2c1, 0x2c2, 0x2c3, 0x2c4, 0x2c5, 0x2c6, 0x2c7,
    0x000, 0x000, 0x2c4, 0x2c5, 0x2c6, 0x2c7, 0x2c4, 0x2c5,
    0x2c6, 0x2c7, 0x000, 0x000, 0x2c8, 0x2c9, 0x2ca, 0x2cb,
    0x2cc, 0x2cd, 0x2ce, 0x2cf, 0x000, 0x000, 0x2cc, 0x2cd,
    0x2ce, 0x2cf, 0x2cc, 0x2cd, 0x2ce, 0x2cf, 0x000, 0x000,
    0x2d0, 0x2d1, 0x2d2, 0x2d3, 0x2d4, 0x2d5, 0x2d6, 0x2d7,
    0x000, 0x000, 0x2d4, 0x2d5, 0x2d6, 0x2d7, 0x2d4, 0x2d5,
    0x2d6, 0x2d7, 0x000, 0x000, 0x2d8, 0x2d9, 0x2da, 0x2db,
    0x2dc, 0x2dd, 0x2de, 0x2df, 0x000, 0x000, 0x2dc, 0x2dd,
    0x2de, 0x2df, 0x2dc, 0x2dd, 0x2de, 0x2df, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x3c0, 0x3c1, 0x3c2, 0x3c3,
    0x3c4, 0x3c5, 0x3c6, 0x3c7, 0x000, 0x000, 0x3c4, 0x3c5,
    0x3c6, 0x3c7, 0x3c4, 0x3c5, 0x3c6, 0x3c7, 0x000, 0x000,
    0x3c8, 0x3c9, 0x3ca, 0x3cb, 0x3cc, 0x3cd, 0x3ce, 0x3cf,
    0x000, 0x000, 0x3cc, 0x3cd, 0x3ce, 0x3cf, 0x3cc, 0x3cd,
    0x3ce, 0x3cf, 0x000, 0x000, 0x3d0, 0x3d1, 0x3d2, 0x3d3,
    0x3d4, 0x3d5, 0x3d6, 0x3d7, 0x000, 0x000, 0x3d4, 0x3d5,
    0x3d6, 0x3d7, 0x3d4, 0x3d5, 0x3d6, 0x3d7, 0x000, 0x000,
    0x3d8, 0x3d9, 0x3da, 0x3db, 0x3dc, 0x3dd, 0x3de, 0x3df,
    0x000, 0x000, 0x3dc, 0x3dd, 0x3de, 0x3df, 0x3dc, 0x3dd,
    0x3de, 0x3df, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000
};

constexpr auto encode_dpd(std::uint8_t d0, std::uint8_t d1, std::uint8_t d2) -> std::uint16_t
{
    const auto bcd {(d2 * 100) + (d1 * 10) + d0};

    BOOST_DECIMAL_ASSERT(bcd >= 0 && bcd < 1000);
    return dpd_table[bcd];
}

} // namespace decimal
} // namespace boost

#endif // BOOST_DECIMAL_DPD_CONVERSION_HPP
