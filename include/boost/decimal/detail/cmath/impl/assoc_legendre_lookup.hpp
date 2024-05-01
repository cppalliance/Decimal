// Copyright 2024 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_ASSOC_LEGENDRE_LOOKUP_HPP
#define BOOST_ASSOC_LEGENDRE_LOOKUP_HPP

#include <boost/decimal/fwd.hpp>
#include <boost/decimal/detail/type_traits.hpp>
#include <boost/decimal/detail/concepts.hpp>
#include <boost/decimal/detail/config.hpp>

#ifndef BOOST_DECIMAL_BUILD_MODULE
#include <type_traits>
#include <array>
#include <cstdint>
#endif

namespace boost {
namespace decimal {
namespace detail {

namespace assoc_legendre_detail {

template <bool b>
struct assoc_legendre_lookup {

    static constexpr std::array<decimal32, 100> d32_values =
    {{
         decimal32{UINT32_C(1000000),-6},
         decimal32{UINT32_C(1000000),-6},
         decimal32{UINT32_C(2000000),-6},
         decimal32{UINT32_C(3000000),-6},
         decimal32{UINT32_C(8000000),-6},
         decimal32{UINT32_C(1500000),-5},
         decimal32{UINT32_C(4800000),-5},
         decimal32{UINT32_C(1050000),-4},
         decimal32{UINT32_C(3840000),-4},
         decimal32{UINT32_C(9450000),-4},
         decimal32{UINT32_C(3840000),-3},
         decimal32{UINT32_C(1039500),-2},
         decimal32{UINT32_C(4608000),-2},
         decimal32{UINT32_C(1351350),-1},
         decimal32{UINT32_C(6451200),-1},
         decimal32{UINT32_C(2027025),0},
         decimal32{UINT32_C(1032192),1},
         decimal32{UINT32_C(3445943),1},
         decimal32{UINT32_C(1857946),2},
         decimal32{UINT32_C(6547291),2},
         decimal32{UINT32_C(3715891),3},
         decimal32{UINT32_C(1374931),4},
         decimal32{UINT32_C(8174961),4},
         decimal32{UINT32_C(3162341),5},
         decimal32{UINT32_C(1961991),6},
         decimal32{UINT32_C(7905854),6},
         decimal32{UINT32_C(5101175),7},
         decimal32{UINT32_C(2134580),8},
         decimal32{UINT32_C(1428329),9},
         decimal32{UINT32_C(6190283),9},
         decimal32{UINT32_C(4284987),10},
         decimal32{UINT32_C(1918988),11},
         decimal32{UINT32_C(1371196),12},
         decimal32{UINT32_C(6332660),12},
         decimal32{UINT32_C(4662066),13},
         decimal32{UINT32_C(2216431),14},
         decimal32{UINT32_C(1678344),15},
         decimal32{UINT32_C(8200795),15},
         decimal32{UINT32_C(6377707),16},
         decimal32{UINT32_C(3198310),17},
         decimal32{UINT32_C(2551083),18},
         decimal32{UINT32_C(1311307),19},
         decimal32{UINT32_C(1071455),20},
         decimal32{UINT32_C(5638620),20},
         decimal32{UINT32_C(4714401),21},
         decimal32{UINT32_C(2537379),22},
         decimal32{UINT32_C(2168624),23},
         decimal32{UINT32_C(1192568),24},
         decimal32{UINT32_C(1040940),25},
         decimal32{UINT32_C(5843584),25},
         decimal32{UINT32_C(5204698),26},
         decimal32{UINT32_C(2980228),27},
         decimal32{UINT32_C(2706443),28},
         decimal32{UINT32_C(1579521),29},
         decimal32{UINT32_C(1461479),30},
         decimal32{UINT32_C(8687364),30},
         decimal32{UINT32_C(8184284),31},
         decimal32{UINT32_C(4951798),32},
         decimal32{UINT32_C(4746885),33},
         decimal32{UINT32_C(2921561),34},
         decimal32{UINT32_C(2848131),35},
         decimal32{UINT32_C(1782152),36},
         decimal32{UINT32_C(1765841),37},
         decimal32{UINT32_C(1122756),38},
         decimal32{UINT32_C(1130138),39},
         decimal32{UINT32_C(7297912),39},
         decimal32{UINT32_C(7458913),40},
         decimal32{UINT32_C(4889601),41},
         decimal32{UINT32_C(5072061),42},
         decimal32{UINT32_C(3373825),43},
         decimal32{UINT32_C(3550443),44},
         decimal32{UINT32_C(2395416),45},
         decimal32{UINT32_C(2556319),46},
         decimal32{UINT32_C(1748653),47},
         decimal32{UINT32_C(1891676),48},
         decimal32{UINT32_C(1311490),49},
         decimal32{UINT32_C(1437674),50},
         decimal32{UINT32_C(1009847),51},
         decimal32{UINT32_C(1121385),52},
         decimal32{UINT32_C(7977794),52},
         decimal32{UINT32_C(8971083),53},
         decimal32{UINT32_C(6462013),54},
         decimal32{UINT32_C(7356288),55},
         decimal32{UINT32_C(5363471),56},
         decimal32{UINT32_C(6179282),57},
         decimal32{UINT32_C(4558950),58},
         decimal32{UINT32_C(5314183),59},
         decimal32{UINT32_C(3966287),60},
         decimal32{UINT32_C(4676481),61},
         decimal32{UINT32_C(3529995),62},
         decimal32{UINT32_C(4208833),63},
         decimal32{UINT32_C(3212296),64},
         decimal32{UINT32_C(3872126),65},
         decimal32{UINT32_C(2987435),66},
         decimal32{UINT32_C(3639799),67},
         decimal32{UINT32_C(2838063),68},
         decimal32{UINT32_C(3494207),69},
         decimal32{UINT32_C(2752921),70},
         decimal32{UINT32_C(3424322),71},
         decimal32{UINT32_C(2725392),72},
    }};


    static constexpr std::array<decimal64, 100> d64_values =
    {{
         decimal64{UINT64_C(1000000000000000),-15},
         decimal64{UINT64_C(1000000000000000),-15},
         decimal64{UINT64_C(2000000000000000),-15},
         decimal64{UINT64_C(3000000000000000),-15},
         decimal64{UINT64_C(8000000000000000),-15},
         decimal64{UINT64_C(1500000000000000),-14},
         decimal64{UINT64_C(4800000000000000),-14},
         decimal64{UINT64_C(1050000000000000),-13},
         decimal64{UINT64_C(3840000000000000),-13},
         decimal64{UINT64_C(9450000000000000),-13},
         decimal64{UINT64_C(3840000000000000),-12},
         decimal64{UINT64_C(1039500000000000),-11},
         decimal64{UINT64_C(4608000000000000),-11},
         decimal64{UINT64_C(1351350000000000),-10},
         decimal64{UINT64_C(6451200000000000),-10},
         decimal64{UINT64_C(2027025000000000),-9},
         decimal64{UINT64_C(1032192000000000),-8},
         decimal64{UINT64_C(3445942500000000),-8},
         decimal64{UINT64_C(1857945600000000),-7},
         decimal64{UINT64_C(6547290750000000),-7},
         decimal64{UINT64_C(3715891200000000),-6},
         decimal64{UINT64_C(1374931057500000),-5},
         decimal64{UINT64_C(8174960640000000),-5},
         decimal64{UINT64_C(3162341432250000),-4},
         decimal64{UINT64_C(1961990553600000),-3},
         decimal64{UINT64_C(7905853580625000),-3},
         decimal64{UINT64_C(5101175439360000),-2},
         decimal64{UINT64_C(2134580466768750),-1},
         decimal64{UINT64_C(1428329123020800),0},
         decimal64{UINT64_C(6190283353629375),0},
         decimal64{UINT64_C(4284987369062400),1},
         decimal64{UINT64_C(1918987839625106),2},
         decimal64{UINT64_C(1371195958099968),3},
         decimal64{UINT64_C(6332659870762850),3},
         decimal64{UINT64_C(4662066257539891),4},
         decimal64{UINT64_C(2216430954766998),5},
         decimal64{UINT64_C(1678343852714361),6},
         decimal64{UINT64_C(8200794532637892),6},
         decimal64{UINT64_C(6377706640314571),7},
         decimal64{UINT64_C(3198309867728778),8},
         decimal64{UINT64_C(2551082656125829),9},
         decimal64{UINT64_C(1311307045768799),10},
         decimal64{UINT64_C(1071454715572848),11},
         decimal64{UINT64_C(5638620296805835),11},
         decimal64{UINT64_C(4714400748520531),12},
         decimal64{UINT64_C(2537379133562626),13},
         decimal64{UINT64_C(2168624344319444),14},
         decimal64{UINT64_C(1192568192774434),15},
         decimal64{UINT64_C(1040939685273333),16},
         decimal64{UINT64_C(5843584144594727),16},
         decimal64{UINT64_C(5204698426366666),17},
         decimal64{UINT64_C(2980227913743311),18},
         decimal64{UINT64_C(2706443181710667),19},
         decimal64{UINT64_C(1579520794283955),20},
         decimal64{UINT64_C(1461479318123760),21},
         decimal64{UINT64_C(8687364368561751),21},
         decimal64{UINT64_C(8184284181493056),22},
         decimal64{UINT64_C(4951797690080198),23},
         decimal64{UINT64_C(4746884825265972),24},
         decimal64{UINT64_C(2921560637147317),25},
         decimal64{UINT64_C(2848130895159583),26},
         decimal64{UINT64_C(1782151988659863),27},
         decimal64{UINT64_C(1765841154998942),28},
         decimal64{UINT64_C(1122755752855714),29},
         decimal64{UINT64_C(1130138339199323),30},
         decimal64{UINT64_C(7297912393562140),30},
         decimal64{UINT64_C(7458913038715529),31},
         decimal64{UINT64_C(4889601303686634),32},
         decimal64{UINT64_C(5072060866326560),33},
         decimal64{UINT64_C(3373824899543778),34},
         decimal64{UINT64_C(3550442606428592),35},
         decimal64{UINT64_C(2395415678676082),36},
         decimal64{UINT64_C(2556318676628587),37},
         decimal64{UINT64_C(1748653445433540),38},
         decimal64{UINT64_C(1891675820705154),39},
         decimal64{UINT64_C(1311490084075155),40},
         decimal64{UINT64_C(1437673623735917),41},
         decimal64{UINT64_C(1009847364737869),42},
         decimal64{UINT64_C(1121385426514015),43},
         decimal64{UINT64_C(7977794181429167),43},
         decimal64{UINT64_C(8971083412112120),44},
         decimal64{UINT64_C(6462013286957625),45},
         decimal64{UINT64_C(7356288397931940),46},
         decimal64{UINT64_C(5363471028174829),47},
         decimal64{UINT64_C(6179282254262830),48},
         decimal64{UINT64_C(4558950373948605),49},
         decimal64{UINT64_C(5314182738666033),50},
         decimal64{UINT64_C(3966286825335287),51},
         decimal64{UINT64_C(4676480810026109),52},
         decimal64{UINT64_C(3529995274548405),53},
         decimal64{UINT64_C(4208832729023498),54},
         decimal64{UINT64_C(3212295699839048),55},
         decimal64{UINT64_C(3872126110701619),56},
         decimal64{UINT64_C(2987435000850315),57},
         decimal64{UINT64_C(3639798544059521),58},
         decimal64{UINT64_C(2838063250807799),59},
         decimal64{UINT64_C(3494206602297140),60},
         decimal64{UINT64_C(2752921353283565),61},
         decimal64{UINT64_C(3424322470251197),62},
         decimal64{UINT64_C(2725392139750730),63},
    }};

    static constexpr std::array<decimal128, 100> d128_values =
    {{
         decimal128{detail::uint128{UINT64_C(54210108624275),UINT64_C(4089650035136921600)},-33},
         decimal128{detail::uint128{UINT64_C(54210108624275),UINT64_C(4089650035136921600)},-33},
         decimal128{detail::uint128{UINT64_C(108420217248550),UINT64_C(8179300070273843200)},-33},
         decimal128{detail::uint128{UINT64_C(162630325872825),UINT64_C(12268950105410764800)},-33},
         decimal128{detail::uint128{UINT64_C(433680868994201),UINT64_C(14270456207385821184)},-33},
         decimal128{detail::uint128{UINT64_C(81315162936412),UINT64_C(15357847089560158208)},-32},
         decimal128{detail::uint128{UINT64_C(260208521396521),UINT64_C(1183576094947672064)},-32},
         decimal128{detail::uint128{UINT64_C(56920614055488),UINT64_C(18129190592175931392)},-31},
         decimal128{detail::uint128{UINT64_C(208166817117216),UINT64_C(15704256134925778944)},-31},
         decimal128{detail::uint128{UINT64_C(512285526499400),UINT64_C(15588762739906969600)},-31},
         decimal128{detail::uint128{UINT64_C(208166817117216),UINT64_C(15704256134925778944)},-30},
         decimal128{detail::uint128{UINT64_C(56351407914934),UINT64_C(1714763901389766656)},-29},
         decimal128{detail::uint128{UINT64_C(249800180540660),UINT64_C(4087712102943293440)},-29},
         decimal128{detail::uint128{UINT64_C(73256830289414),UINT64_C(5918541886548606976)},-28},
         decimal128{detail::uint128{UINT64_C(349720252756924),UINT64_C(5722796944120610816)},-28},
         decimal128{detail::uint128{UINT64_C(109885245434121),UINT64_C(8877812829822910464)},-27},
         decimal128{detail::uint128{UINT64_C(55955240441107),UINT64_C(16410912532975321088)},-26},
         decimal128{detail::uint128{UINT64_C(186804917238006),UINT64_C(9558258588586082304)},-26},
         decimal128{detail::uint128{UINT64_C(100719432793994),UINT64_C(3714200856162205696)},-25},
         decimal128{detail::uint128{UINT64_C(354929342752212),UINT64_C(7092644874087825408)},-25},
         decimal128{detail::uint128{UINT64_C(201438865587988),UINT64_C(7428401712324411392)},-24},
         decimal128{detail::uint128{UINT64_C(74535161977964),UINT64_C(11081762341887410176)},-23},
         decimal128{detail::uint128{UINT64_C(443165504293574),UINT64_C(8963786137629884416)},-23},
         decimal128{detail::uint128{UINT64_C(171430872549318),UINT64_C(10730658127373402112)},-22},
         decimal128{detail::uint128{UINT64_C(106359721030457),UINT64_C(16170834169050431488)},-21},
         decimal128{detail::uint128{UINT64_C(428577181373296),UINT64_C(8379901244723953664)},-21},
         decimal128{detail::uint128{UINT64_C(276535274679190),UINT64_C(8840029506853928960)},-20},
         decimal128{detail::uint128{UINT64_C(115715838970790),UINT64_C(786833810178703360)},-19},
         decimal128{detail::uint128{UINT64_C(77429876910173),UINT64_C(6164557076661010432)},-18},
         decimal128{detail::uint128{UINT64_C(335575933015291),UINT64_C(2281818049518239744)},-18},
         decimal128{detail::uint128{UINT64_C(232289630730520),UINT64_C(46927156273479680)},-17},
         decimal128{detail::uint128{UINT64_C(104028539234740),UINT64_C(4531179850829660160)},-16},
         decimal128{detail::uint128{UINT64_C(74332681833766),UINT64_C(7393714319491334144)},-15},
         decimal128{detail::uint128{UINT64_C(343294179474642),UINT64_C(14492893507737878528)},-15},
         decimal128{detail::uint128{UINT64_C(252731118234805),UINT64_C(13870582242044805120)},-14},
         decimal128{detail::uint128{UINT64_C(120152962816124),UINT64_C(18085233579304943616)},-13},
         decimal128{detail::uint128{UINT64_C(90983202564530),UINT64_C(1344060792394219520)},-12},
         decimal128{detail::uint128{UINT64_C(444565962419662),UINT64_C(8765783207557726208)},-12},
         decimal128{detail::uint128{UINT64_C(345736169745214),UINT64_C(5067431011098034176)},-11},
         decimal128{detail::uint128{UINT64_C(173380725343668),UINT64_C(6359069384215232512)},-10},
         decimal128{detail::uint128{UINT64_C(138294467898085),UINT64_C(13195018848664944640)},-9},
         decimal128{detail::uint128{UINT64_C(71086097390904),UINT64_C(418609158683099136)},-8},
         decimal128{detail::uint128{UINT64_C(58083676517196),UINT64_C(37884694326411264)},-7},
         decimal128{detail::uint128{UINT64_C(305670218780887),UINT64_C(5649368197079236608)},-7},
         decimal128{detail::uint128{UINT64_C(255568176675662),UINT64_C(7345390284520030208)},-6},
         decimal128{detail::uint128{UINT64_C(137551598451399),UINT64_C(5159227299742089216)},-5},
         decimal128{detail::uint128{UINT64_C(117561361270804),UINT64_C(13111186449208180736)},-4},
         decimal128{detail::uint128{UINT64_C(64649251272157),UINT64_C(12369611189944844288)},-3},
         decimal128{detail::uint128{UINT64_C(56429453409986),UINT64_C(4705629969723162624)},-2},
         decimal128{detail::uint128{UINT64_C(316781331233572),UINT64_C(10224885831713947648)},-2},
         decimal128{detail::uint128{UINT64_C(282147267049931),UINT64_C(5081405774906261504)},-1},
         decimal128{detail::uint128{UINT64_C(161558478929122),UINT64_C(279603433535438848)},0},
         decimal128{detail::uint128{UINT64_C(146716578865964),UINT64_C(5035940291796402176)},1},
         decimal128{detail::uint128{UINT64_C(85625993832434),UINT64_C(12193040908422086656)},2},
         decimal128{detail::uint128{UINT64_C(79226952587620),UINT64_C(12939584438847406080)},3},
         decimal128{detail::uint128{UINT64_C(470942966078390),UINT64_C(11871492775192821760)},3},
         decimal128{detail::uint128{UINT64_C(443670934490675),UINT64_C(18241440636416819200)},4},
         decimal128{detail::uint128{UINT64_C(268437490664682),UINT64_C(12230774103972773888)},5},
         decimal128{detail::uint128{UINT64_C(257329142004592),UINT64_C(876663532266979328)},6},
         decimal128{detail::uint128{UINT64_C(158378119492162),UINT64_C(14405919469353566208)},7},
         decimal128{detail::uint128{UINT64_C(154397485202755),UINT64_C(4415346934102097920)},8},
         decimal128{detail::uint128{UINT64_C(96610652890219),UINT64_C(5497196943037956096)},9},
         decimal128{detail::uint128{UINT64_C(95726440825708),UINT64_C(4374189506514255872)},10},
         decimal128{detail::uint128{UINT64_C(60864711320838),UINT64_C(2767831751902625792)},11},
         decimal128{detail::uint128{UINT64_C(61264922128453),UINT64_C(5053090573014269952)},12},
         decimal128{detail::uint128{UINT64_C(395620623585447),UINT64_C(18290906387367067648)},12},
         decimal128{detail::uint128{UINT64_C(404348486047791),UINT64_C(11054304893442719744)},13},
         decimal128{detail::uint128{UINT64_C(265065817802250),UINT64_C(3047067801944064000)},14},
         decimal128{detail::uint128{UINT64_C(274956970512498),UINT64_C(5583318038695903232)},15},
         decimal128{detail::uint128{UINT64_C(182895414283552),UINT64_C(11365848820196179968)},16},
         decimal128{detail::uint128{UINT64_C(192469879358748),UINT64_C(14976369071312863232)},17},
         decimal128{detail::uint128{UINT64_C(129855744141322),UINT64_C(6569013136442523648)},18},
         decimal128{detail::uint128{UINT64_C(138578313138299),UINT64_C(2926418338913058816)},19},
         decimal128{detail::uint128{UINT64_C(94794693223165),UINT64_C(5842184234025615360)},20},
         decimal128{detail::uint128{UINT64_C(102547951722341),UINT64_C(6951703029960146944)},21},
         decimal128{detail::uint128{UINT64_C(71096019917373),UINT64_C(18166696230801375232)},22},
         decimal128{detail::uint128{UINT64_C(77936443308979),UINT64_C(8194773354563239936)},23},
         decimal128{detail::uint128{UINT64_C(54743935336377),UINT64_C(17966172353196064768)},24},
         decimal128{detail::uint128{UINT64_C(60790425781003),UINT64_C(17768904542259249152)},25},
         decimal128{detail::uint128{UINT64_C(432477089157385),UINT64_C(17869576296394915840)},25},
         decimal128{detail::uint128{UINT64_C(486323406248031),UINT64_C(11424027822107131904)},26},
         decimal128{detail::uint128{UINT64_C(350306442217482),UINT64_C(11437345189023449088)},27},
         decimal128{detail::uint128{UINT64_C(398785193123386),UINT64_C(268591251376308224)},28},
         decimal128{detail::uint128{UINT64_C(290754347040510),UINT64_C(10849801151312035840)},29},
         decimal128{detail::uint128{UINT64_C(334979562223644),UINT64_C(4552835228846391296)},30},
         decimal128{detail::uint128{UINT64_C(247141194984434),UINT64_C(348958941760454656)},31},
         decimal128{detail::uint128{UINT64_C(288082423512334),UINT64_C(593959245014368256)},32},
         decimal128{detail::uint128{UINT64_C(215012839636457),UINT64_C(11152705842083135488)},33},
         decimal128{detail::uint128{UINT64_C(253512532690853),UINT64_C(17753688683425431552)},34},
         decimal128{detail::uint128{UINT64_C(191361427276447),UINT64_C(4960287299552411648)},35},
         decimal128{detail::uint128{UINT64_C(228161279421768),UINT64_C(10074296592970022912)},36},
         decimal128{detail::uint128{UINT64_C(174138898821566),UINT64_C(18167854379349049344)},37},
         decimal128{detail::uint128{UINT64_C(209908377068027),UINT64_C(1491785473100218368)},38},
         decimal128{detail::uint128{UINT64_C(161949175904057),UINT64_C(5819123247094693888)},39},
         decimal128{detail::uint128{UINT64_C(197313874443945),UINT64_C(8022041092723834880)},40},
         decimal128{detail::uint128{UINT64_C(153851717108854),UINT64_C(8045178695796391936)},41},
         decimal128{detail::uint128{UINT64_C(189421319466187),UINT64_C(11630508263756791808)},42},
         decimal128{detail::uint128{UINT64_C(149236165595588),UINT64_C(14983586082932129792)},43},
         decimal128{detail::uint128{UINT64_C(185632893076863),UINT64_C(16002051557646139392)},44},
         decimal128{detail::uint128{UINT64_C(147743803939632),UINT64_C(16999359510947954688)},45},
    }};
};

#if !(defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L) && (!defined(_MSC_VER) || _MSC_VER != 1900)

template <bool b>
constexpr std::array<decimal32, 100> assoc_legendre_lookup<b>::d32_values;

template <bool b>
constexpr std::array<decimal64, 100> assoc_legendre_lookup<b>::d64_values;

template <bool b>
constexpr std::array<decimal128, 100> assoc_legendre_lookup<b>::d128_values;

#endif

using assoc_legendre_lookup_table = assoc_legendre_lookup<true>;

} //namespace assoc_legendre_detail

template <BOOST_DECIMAL_DECIMAL_FLOATING_TYPE T>
constexpr auto assoc_legendre_p0_lookup(unsigned n) -> T;

template <>
constexpr auto assoc_legendre_p0_lookup<decimal32>(unsigned n) -> decimal32
{
    return assoc_legendre_detail::assoc_legendre_lookup_table::d32_values[static_cast<std::size_t>(n)];
}

template <>
constexpr auto assoc_legendre_p0_lookup<decimal64>(unsigned n) -> decimal64
{
    return assoc_legendre_detail::assoc_legendre_lookup_table::d64_values[static_cast<std::size_t>(n)];
}

template <>
constexpr auto assoc_legendre_p0_lookup<decimal128>(unsigned n) -> decimal128
{
    return assoc_legendre_detail::assoc_legendre_lookup_table::d128_values[static_cast<std::size_t>(n)];
}

} //namespace detail
} //namespace decimal
} //namespace boost

#endif //BOOST_ASSOC_LEGENDRE_LOOKUP_HPP
