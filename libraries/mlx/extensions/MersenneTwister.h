#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include <stdint.h>

typedef struct {
    uint32_t state[624];
    int index;
} mt19937_t;

void mt19937_init(mt19937_t *mt, uint32_t seed);
float mt19937_float(mt19937_t *mt);
float mt19937_float_range(mt19937_t *mt, float min_val, float max_val);

#endif // MERSENNE_TWISTER_H
