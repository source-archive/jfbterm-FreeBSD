/*
 * JFBTERM for FreeBSD
 * Copyright (C) 2009 Yusuke BABA <babayaga1@y8.dion.ne.jp>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY YUSUKE BABA ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL YUSUKE BABA BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ENABLE_UTF8

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

#include "eastasianwidth.h"

struct interval {
	uint32_t first;
	uint32_t last;
};

/*
 * EastAsianWidth-5.1.0.txt
 */

static const struct interval eastAsianAmbiguous[] = {
	{ 0x00a1, 0x00a1 }, { 0x00a4, 0x00a4 }, { 0x00a7, 0x00a8 },
	{ 0x00aa, 0x00aa }, { 0x00ad, 0x00ae }, { 0x00b0, 0x00b4 },
	{ 0x00b6, 0x00ba }, { 0x00bc, 0x00bf }, { 0x00c6, 0x00c6 },
	{ 0x00d0, 0x00d0 }, { 0x00d7, 0x00d8 }, { 0x00de, 0x00e1 },
	{ 0x00e6, 0x00e6 }, { 0x00e8, 0x00ea }, { 0x00ec, 0x00ed },
	{ 0x00f0, 0x00f0 }, { 0x00f2, 0x00f3 }, { 0x00f7, 0x00fa },
	{ 0x00fc, 0x00fc }, { 0x00fe, 0x00fe }, { 0x0101, 0x0101 },
	{ 0x0111, 0x0111 }, { 0x0113, 0x0113 }, { 0x011b, 0x011b },
	{ 0x0126, 0x0127 }, { 0x012b, 0x012b }, { 0x0131, 0x0133 },
	{ 0x0138, 0x0138 }, { 0x013f, 0x0142 }, { 0x0144, 0x0144 },
	{ 0x0148, 0x014b }, { 0x014d, 0x014d }, { 0x0152, 0x0153 },
	{ 0x0166, 0x0167 }, { 0x016b, 0x016b }, { 0x01ce, 0x01ce },
	{ 0x01d0, 0x01d0 }, { 0x01d2, 0x01d2 }, { 0x01d4, 0x01d4 },
	{ 0x01d6, 0x01d6 }, { 0x01d8, 0x01d8 }, { 0x01da, 0x01da },
	{ 0x01dc, 0x01dc }, { 0x0251, 0x0251 }, { 0x0261, 0x0261 },
	{ 0x02c4, 0x02c4 }, { 0x02c7, 0x02c7 }, { 0x02c9, 0x02cb },
	{ 0x02cd, 0x02cd }, { 0x02d0, 0x02d0 }, { 0x02d8, 0x02db },
	{ 0x02dd, 0x02dd }, { 0x02df, 0x02df }, { 0x0300, 0x036f },
	{ 0x0391, 0x03a9 }, { 0x03b1, 0x03c1 }, { 0x03c3, 0x03c9 },
	{ 0x0401, 0x0401 }, { 0x0410, 0x044f }, { 0x0451, 0x0451 },
	{ 0x2010, 0x2010 }, { 0x2013, 0x2016 }, { 0x2018, 0x2019 },
	{ 0x201c, 0x201d }, { 0x2020, 0x2022 }, { 0x2024, 0x2027 },
	{ 0x2030, 0x2030 }, { 0x2032, 0x2033 }, { 0x2035, 0x2035 },
	{ 0x203b, 0x203b }, { 0x203e, 0x203e }, { 0x2074, 0x2074 },
	{ 0x207f, 0x207f }, { 0x2081, 0x2084 }, { 0x20ac, 0x20ac },
	{ 0x2103, 0x2103 }, { 0x2105, 0x2105 }, { 0x2109, 0x2109 },
	{ 0x2113, 0x2113 }, { 0x2116, 0x2116 }, { 0x2121, 0x2122 },
	{ 0x2126, 0x2126 }, { 0x212b, 0x212b }, { 0x2153, 0x2154 },
	{ 0x215b, 0x215e }, { 0x2160, 0x216b }, { 0x2170, 0x2179 },
	{ 0x2190, 0x2199 }, { 0x21b8, 0x21b9 }, { 0x21d2, 0x21d2 },
	{ 0x21d4, 0x21d4 }, { 0x21e7, 0x21e7 }, { 0x2200, 0x2200 },
	{ 0x2202, 0x2203 }, { 0x2207, 0x2208 }, { 0x220b, 0x220b },
	{ 0x220f, 0x220f }, { 0x2211, 0x2211 }, { 0x2215, 0x2215 },
	{ 0x221a, 0x221a }, { 0x221d, 0x2220 }, { 0x2223, 0x2223 },
	{ 0x2225, 0x2225 }, { 0x2227, 0x222c }, { 0x222e, 0x222e },
	{ 0x2234, 0x2237 }, { 0x223c, 0x223d }, { 0x2248, 0x2248 },
	{ 0x224c, 0x224c }, { 0x2252, 0x2252 }, { 0x2260, 0x2261 },
	{ 0x2264, 0x2267 }, { 0x226a, 0x226b }, { 0x226e, 0x226f },
	{ 0x2282, 0x2283 }, { 0x2286, 0x2287 }, { 0x2295, 0x2295 },
	{ 0x2299, 0x2299 }, { 0x22a5, 0x22a5 }, { 0x22bf, 0x22bf },
	{ 0x2312, 0x2312 }, { 0x2460, 0x24e9 }, { 0x24eb, 0x254b },
	{ 0x2550, 0x2573 }, { 0x2580, 0x258f }, { 0x2592, 0x2595 },
	{ 0x25a0, 0x25a1 }, { 0x25a3, 0x25a9 }, { 0x25b2, 0x25b3 },
	{ 0x25b6, 0x25b7 }, { 0x25bc, 0x25bd }, { 0x25c0, 0x25c1 },
	{ 0x25c6, 0x25c8 }, { 0x25cb, 0x25cb }, { 0x25ce, 0x25d1 },
	{ 0x25e2, 0x25e5 }, { 0x25ef, 0x25ef }, { 0x2605, 0x2606 },
	{ 0x2609, 0x2609 }, { 0x260e, 0x260f }, { 0x2614, 0x2615 },
	{ 0x261c, 0x261c }, { 0x261e, 0x261e }, { 0x2640, 0x2640 },
	{ 0x2642, 0x2642 }, { 0x2660, 0x2661 }, { 0x2663, 0x2665 },
	{ 0x2667, 0x266a }, { 0x266c, 0x266d }, { 0x266f, 0x266f },
	{ 0x273d, 0x273d }, { 0x2776, 0x277f }, { 0xe000, 0xf8ff },
	{ 0xfe00, 0xfe0f }, { 0xfffd, 0xfffd },
	{ 0xe0100, 0xe01ef },
	{ 0xf0000, 0xffffd },
	{ 0x100000, 0x10fffd }
};

static const struct interval eastAsianFullWidth[] = {
	{ 0x3000, 0x3000 }, { 0xff01, 0xff60 }, { 0xffe0, 0xffe6 }
};

static const struct interval eastAsianHalfWidth[] = {
	{ 0x20a9, 0x20a9 }, { 0xff61, 0xffdc }, { 0xffe8, 0xffee }
};

static const struct interval eastAsianNarrow[] = {
	{ 0x0020, 0x007e }, { 0x00a2, 0x00a3 }, { 0x00a5, 0x00a6 },
	{ 0x00ac, 0x00ac }, { 0x00af, 0x00af }, { 0x27e6, 0x27ed },
	{ 0x2985, 0x2986 }
};

static const struct interval eastAsianNeutral[] = {
	{ 0x0000, 0x001f }, { 0x007f, 0x00a0 }, { 0x00a9, 0x00a9 },
	{ 0x00ab, 0x00ab }, { 0x00b5, 0x00b5 }, { 0x00bb, 0x00bb },
	{ 0x00c0, 0x00c5 }, { 0x00c7, 0x00cf }, { 0x00d1, 0x00d6 },
	{ 0x00d9, 0x00dd }, { 0x00e2, 0x00e5 }, { 0x00e7, 0x00e7 },
	{ 0x00eb, 0x00eb }, { 0x00ee, 0x00ef }, { 0x00f1, 0x00f1 },
	{ 0x00f4, 0x00f6 }, { 0x00fb, 0x00fb }, { 0x00fd, 0x00fd },
	{ 0x00ff, 0x0100 }, { 0x0102, 0x0110 }, { 0x0112, 0x0112 },
	{ 0x0114, 0x011a }, { 0x011c, 0x0125 }, { 0x0128, 0x012a },
	{ 0x012c, 0x0130 }, { 0x0134, 0x0137 }, { 0x0139, 0x013e },
	{ 0x0143, 0x0143 }, { 0x0145, 0x0147 }, { 0x014c, 0x014c },
	{ 0x014e, 0x0151 }, { 0x0154, 0x0165 }, { 0x0168, 0x016a },
	{ 0x016c, 0x01cd }, { 0x01cf, 0x01cf }, { 0x01d1, 0x01d1 },
	{ 0x01d3, 0x01d3 }, { 0x01d5, 0x01d5 }, { 0x01d7, 0x01d7 },
	{ 0x01d9, 0x01d9 }, { 0x01db, 0x01db }, { 0x01dd, 0x0250 },
	{ 0x0252, 0x0260 }, { 0x0262, 0x02c3 }, { 0x02c5, 0x02c6 },
	{ 0x02c8, 0x02c8 }, { 0x02cc, 0x02cc }, { 0x02ce, 0x02cf },
	{ 0x02d1, 0x02d7 }, { 0x02dc, 0x02dc }, { 0x02de, 0x02de },
	{ 0x02e0, 0x02ff }, { 0x0370, 0x0390 }, { 0x03aa, 0x03b0 },
	{ 0x03c2, 0x03c2 }, { 0x03ca, 0x0400 }, { 0x0402, 0x040f },
	{ 0x0450, 0x0450 }, { 0x0452, 0x10fc }, { 0x1160, 0x200f },
	{ 0x2011, 0x2012 }, { 0x2017, 0x2017 }, { 0x201a, 0x201b },
	{ 0x201e, 0x201f }, { 0x2023, 0x2023 }, { 0x2028, 0x202f },
	{ 0x2031, 0x2031 }, { 0x2034, 0x2034 }, { 0x2036, 0x203a },
	{ 0x203c, 0x203d }, { 0x203f, 0x2071 }, { 0x2075, 0x207e },
	{ 0x2080, 0x2080 }, { 0x2085, 0x20a8 }, { 0x20aa, 0x20ab },
	{ 0x20ad, 0x2102 }, { 0x2104, 0x2104 }, { 0x2106, 0x2108 },
	{ 0x210a, 0x2112 }, { 0x2114, 0x2115 }, { 0x2117, 0x2120 },
	{ 0x2123, 0x2125 }, { 0x2127, 0x212a }, { 0x212c, 0x214f },
	{ 0x2155, 0x215a }, { 0x215f, 0x215f }, { 0x216c, 0x216f },
	{ 0x217a, 0x2188 }, { 0x219a, 0x21b7 }, { 0x21ba, 0x21d1 },
	{ 0x21d3, 0x21d3 }, { 0x21d5, 0x21e6 }, { 0x21e8, 0x21ff },
	{ 0x2201, 0x2201 }, { 0x2204, 0x2206 }, { 0x2209, 0x220a },
	{ 0x220c, 0x220e }, { 0x2210, 0x2210 }, { 0x2212, 0x2214 },
	{ 0x2216, 0x2219 }, { 0x221b, 0x221c }, { 0x2221, 0x2222 },
	{ 0x2224, 0x2224 }, { 0x2226, 0x2226 }, { 0x222d, 0x222d },
	{ 0x222f, 0x2233 }, { 0x2238, 0x223b }, { 0x223e, 0x2247 },
	{ 0x2249, 0x224b }, { 0x224d, 0x2251 }, { 0x2253, 0x225f },
	{ 0x2262, 0x2263 }, { 0x2268, 0x2269 }, { 0x226c, 0x226d },
	{ 0x2270, 0x2281 }, { 0x2284, 0x2285 }, { 0x2288, 0x2294 },
	{ 0x2296, 0x2298 }, { 0x229a, 0x22a4 }, { 0x22a6, 0x22be },
	{ 0x22c0, 0x2311 }, { 0x2313, 0x2328 }, { 0x232b, 0x244a },
	{ 0x24ea, 0x24ea }, { 0x254c, 0x254f }, { 0x2574, 0x257f },
	{ 0x2590, 0x2591 }, { 0x2596, 0x259f }, { 0x25a2, 0x25a2 },
	{ 0x25aa, 0x25b1 }, { 0x25b4, 0x25b5 }, { 0x25b8, 0x25bb },
	{ 0x25be, 0x25bf }, { 0x25c2, 0x25c5 }, { 0x25c9, 0x25ca },
	{ 0x25cc, 0x25cd }, { 0x25d2, 0x25e1 }, { 0x25e6, 0x25ee },
	{ 0x25f0, 0x2604 }, { 0x2607, 0x2608 }, { 0x260a, 0x260d },
	{ 0x2610, 0x2613 }, { 0x2616, 0x261b }, { 0x261d, 0x261d },
	{ 0x261f, 0x263f }, { 0x2641, 0x2641 }, { 0x2643, 0x265f },
	{ 0x2662, 0x2662 }, { 0x2666, 0x2666 }, { 0x266b, 0x266b },
	{ 0x266e, 0x266e }, { 0x2670, 0x273c }, { 0x273e, 0x2775 },
	{ 0x2780, 0x27e5 }, { 0x27ee, 0x2984 }, { 0x2987, 0x2e30 },
	{ 0x303f, 0x303f }, { 0x4dc0, 0x4dff }, { 0xa500, 0xaa5f },
	{ 0xd800, 0xdb7f }, { 0xdb80, 0xdbff }, { 0xdc00, 0xdfff },
	{ 0xfb00, 0xfdfd }, { 0xfe20, 0xfe26 }, { 0xfe70, 0xfeff },
	{ 0xfff9, 0xfffc },
	{ 0x10000, 0x1f093 },
	{ 0xe0001, 0xe007f }
};

static const struct interval eastAsianWide[] = {
	{ 0x1100, 0x115f }, { 0x2329, 0x232a }, { 0x2e80, 0x2ffb },
	{ 0x3001, 0x303e }, { 0x3041, 0x33ff }, { 0x3400, 0x4db5 },
	{ 0x4e00, 0x9fc3 }, { 0xa000, 0xa4c6 }, { 0xac00, 0xd7a3 },
	{ 0xf900, 0xfad9 }, { 0xfe10, 0xfe19 }, { 0xfe30, 0xfe6b },
	{ 0x20000, 0x2a6d6 },
	{ 0x2a6d7, 0x2f7ff },
	{ 0x2f800, 0x2fa1d },
	{ 0x2fa1e, 0x2fffd },
	{ 0x30000, 0x3fffd }
};

static bool bisearch(uint32_t ucs, const struct interval *table, int max);

static bool bisearch(uint32_t ucs, const struct interval *table, int max)
{
	int min, mid;

	if (ucs < table[0].first || ucs > table[max].last)
		return false;
	min = 0;
	while (max >= min) {
		mid = (min + max) / 2;
		if (ucs > table[mid].last)
			min = mid + 1;
		else if (ucs < table[mid].first)
			max = mid - 1;
		else
			return true;
	}
	return false;
}

bool eastasianwidth_isAmbiguous(uint32_t ucs)
{
	return bisearch(ucs, eastAsianAmbiguous,
	       sizeof(eastAsianAmbiguous) / sizeof(struct interval) - 1);
}

bool eastasianwidth_isFullWidth(uint32_t ucs)
{
	return bisearch(ucs, eastAsianFullWidth,
	       sizeof(eastAsianFullWidth) / sizeof(struct interval) - 1);
}

bool eastasianwidth_isHalfWidth(uint32_t ucs)
{
	return bisearch(ucs, eastAsianHalfWidth,
	       sizeof(eastAsianHalfWidth) / sizeof(struct interval) - 1);
}

bool eastasianwidth_isNarrow(uint32_t ucs)
{
	return bisearch(ucs, eastAsianNarrow,
	       sizeof(eastAsianNarrow) / sizeof(struct interval) - 1);
}

bool eastasianwidth_isNeutral(uint32_t ucs)
{
	return bisearch(ucs, eastAsianNeutral,
	       sizeof(eastAsianNeutral) / sizeof(struct interval) - 1);
}

bool eastasianwidth_isWide(uint32_t ucs)
{
	return bisearch(ucs, eastAsianWide,
	       sizeof(eastAsianWide) / sizeof(struct interval) - 1);
}

#endif /* ENABLE_UTF8 */

