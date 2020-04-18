//
// A few 64-bit integer functions that use only 32-bit arithmetic
//
// If you are porting to a small PIC, maybe leave out any
// calls to print functions and run through the debugger.
//
// This project was compiled on a PC with GNU gcc
//
// davekw7x
//

#ifndef UINT64_H__
#define UINT64_H__

#include <stdint.h>

typedef struct uint64 {
    uint32_t low;
    uint32_t high;
} uint64;

int uint64_is_zero(uint64 );
uint64 and_uint64(uint64 *x, uint64 *y);
uint64 make_uint64(uint32_t);

void set_zero_uint64(uint64 *);

uint64 multiply_uint32x32(uint32_t *multiplicand, uint32_t *multiplier);
uint64 shift_left_one_uint64(uint64*);
uint64 shift_left_uint64(uint64*, uint8_t);
uint64 shift_right_one_uint64(uint64*);

uint64 add_uint64(uint64* augend, uint64* addend);
uint64 subtract_uint64(uint64 *minuend, uint64 *subtrahend);
uint64 divide_uint64_by_uint32(uint64 *numerator, uint32_t *denominator, uint32_t *remainder);
int compare_uint64(uint64 *first, uint64 *second);
void print_uint64(uint64 *x);

#endif
