#ifndef __STDC_VERSION_STDBIT_H__
#define __STDC_VERSION_STDBIT_H__ 202311L

#include <alltypes.h>
#include <features.h>
#include <stdbool.h>

#define __STDC_ENDIAN_LITTLE__ 1234
#define __STDC_ENDIAN_BIG__ 4321
#ifdef __BIG_ENDIAN
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_BIG__
#else
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__
#endif

#define __stdbit_select(stem, x) _Generic((x), unsigned char: stem##_uc, unsigned short: stem##_us, unsigned: stem##_ui, unsigned long: stem##_ul, unsigned long long: stem##_ull, \
        unsigned _BitInt(8): stem##_uc, unsigned _BitInt(16): stem##_us, unsigned _BitInt(32): stem##_ui, unsigned _BitInt(8*sizeof(long)): stem##_ul, unsigned _BitInt(64): stem##_ull)(x)

unsigned stdc_leading_zeroes_uc(unsigned char) __unsequenced;
unsigned stdc_leading_zeroes_us(unsigned short) __unsequenced;
unsigned stdc_leading_zeroes_ui(unsigned) __unsequenced;
unsigned stdc_leading_zeroes_ul(unsigned long) __unsequenced;
unsigned stdc_leading_zeroes_ull(unsigned long long) __unsequenced;
#define stdc_leading_zeroes(x) __stdbit_select(stdc_leading_zeroes, (x))

unsigned stdc_leading_ones_uc(unsigned char) __unsequenced;
unsigned stdc_leading_ones_us(unsigned short) __unsequenced;
unsigned stdc_leading_ones_ui(unsigned) __unsequenced;
unsigned stdc_leading_ones_ul(unsigned long) __unsequenced;
unsigned stdc_leading_ones_ull(unsigned long long) __unsequenced;
#define stdc_leading_ones(x) __stdbit_select(stdc_leading_ones, (x))

unsigned stdc_trailing_zeroes_uc(unsigned char) __unsequenced;
unsigned stdc_trailing_zeroes_us(unsigned short) __unsequenced;
unsigned stdc_trailing_zeroes_ui(unsigned) __unsequenced;
unsigned stdc_trailing_zeroes_ul(unsigned long) __unsequenced;
unsigned stdc_trailing_zeroes_ull(unsigned long long) __unsequenced;
#define stdc_trailing_zeroes(x) __stdbit_select(stdc_trailing_zeroes, (x))

unsigned stdc_trailing_ones_uc(unsigned char) __unsequenced;
unsigned stdc_trailing_ones_us(unsigned short) __unsequenced;
unsigned stdc_trailing_ones_ui(unsigned) __unsequenced;
unsigned stdc_trailing_ones_ul(unsigned long) __unsequenced;
unsigned stdc_trailing_ones_ull(unsigned long long) __unsequenced;
#define stdc_trailing_ones(x) __stdbit_select(stdc_trailing_ones, (x))

unsigned stdc_first_leading_zero_uc(unsigned char) __unsequenced;
unsigned stdc_first_leading_zero_us(unsigned short) __unsequenced;
unsigned stdc_first_leading_zero_ui(unsigned) __unsequenced;
unsigned stdc_first_leading_zero_ul(unsigned long) __unsequenced;
unsigned stdc_first_leading_zero_ull(unsigned long long) __unsequenced;
#define stdc_first_leading_zero(x) __stdbit_select(stdc_first_leading_zero, (x))

unsigned stdc_first_leading_one_uc(unsigned char) __unsequenced;
unsigned stdc_first_leading_one_us(unsigned short) __unsequenced;
unsigned stdc_first_leading_one_ui(unsigned) __unsequenced;
unsigned stdc_first_leading_one_ul(unsigned long) __unsequenced;
unsigned stdc_first_leading_one_ull(unsigned long long) __unsequenced;
#define stdc_first_leading_one(x) __stdbit_select(stdc_first_leading_one, (x))

unsigned stdc_first_trailing_zero_uc(unsigned char) __unsequenced;
unsigned stdc_first_trailing_zero_us(unsigned short) __unsequenced;
unsigned stdc_first_trailing_zero_ui(unsigned) __unsequenced;
unsigned stdc_first_trailing_zero_ul(unsigned long) __unsequenced;
unsigned stdc_first_trailing_zero_ull(unsigned long long) __unsequenced;
#define stdc_first_trailing_zero(x) __stdbit_select(stdc_first_trailing_zero, (x))

unsigned stdc_first_trailing_one_uc(unsigned char) __unsequenced;
unsigned stdc_first_trailing_one_us(unsigned short) __unsequenced;
unsigned stdc_first_trailing_one_ui(unsigned) __unsequenced;
unsigned stdc_first_trailing_one_ul(unsigned long) __unsequenced;
unsigned stdc_first_trailing_one_ull(unsigned long long) __unsequenced;
#define stdc_first_trailing_one(x) __stdbit_select(stdc_first_trailing_one, (x))

unsigned stdc_count_zeroes_uc(unsigned char) __unsequenced;
unsigned stdc_count_zeroes_us(unsigned short) __unsequenced;
unsigned stdc_count_zeroes_ui(unsigned) __unsequenced;
unsigned stdc_count_zeroes_ul(unsigned long) __unsequenced;
unsigned stdc_count_zeroes_ull(unsigned long long) __unsequenced;
#define stdc_count_zeroes(x) __stdbit_select(stdc_count_zeroes, (x))

unsigned stdc_count_ones_uc(unsigned char) __unsequenced;
unsigned stdc_count_ones_us(unsigned short) __unsequenced;
unsigned stdc_count_ones_ui(unsigned) __unsequenced;
unsigned stdc_count_ones_ul(unsigned long) __unsequenced;
unsigned stdc_count_ones_ull(unsigned long long) __unsequenced;
#define stdc_count_ones(x) __stdbit_select(stdc_count_ones, (x))

bool stdc_has_single_bit_uc(unsigned char) __unsequenced;
bool stdc_has_single_bit_us(unsigned short) __unsequenced;
bool stdc_has_single_bit_ui(unsigned) __unsequenced;
bool stdc_has_single_bit_ul(unsigned long) __unsequenced;
bool stdc_has_single_bit_ull(unsigned long long) __unsequenced;
#define stdc_has_single_bit(x) __stdbit_select(stdc_has_single_bit, (x))

unsigned stdc_bit_width_uc(unsigned char) __unsequenced;
unsigned stdc_bit_width_us(unsigned short) __unsequenced;
unsigned stdc_bit_width_ui(unsigned) __unsequenced;
unsigned stdc_bit_width_ul(unsigned long) __unsequenced;
unsigned stdc_bit_width_ull(unsigned long long) __unsequenced;
#define stdc_bit_width(x) __stdbit_select(stdc_bit_width, (x))

unsigned char stdc_bit_floor_uc(unsigned char) __unsequenced;
unsigned short stdc_bit_floor_us(unsigned short) __unsequenced;
unsigned stdc_bit_floor_ui(unsigned) __unsequenced;
unsigned long stdc_bit_floor_ul(unsigned long) __unsequenced;
unsigned long long stdc_bit_floor_ull(unsigned long long) __unsequenced;
#define stdc_bit_floor(x) __stdbit_select(stdc_bit_floor, (x))

unsigned char stdc_bit_ceil_uc(unsigned char) __unsequenced;
unsigned short stdc_bit_ceil_us(unsigned short) __unsequenced;
unsigned stdc_bit_ceil_ui(unsigned) __unsequenced;
unsigned long stdc_bit_ceil_ul(unsigned long) __unsequenced;
unsigned long long stdc_bit_ceil_ull(unsigned long long) __unsequenced;
#define stdc_bit_ceil(x) __stdbit_select(stdc_bit_ceil, (x))

#endif
