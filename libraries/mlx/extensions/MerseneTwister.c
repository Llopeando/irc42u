#include "MersenneTwister.h"

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_UPPER_MASK 0x80000000
#define MT19937_LOWER_MASK 0x7fffffff
#define MT19937_TEMPERING_MASK_B 0x9d2c5680
#define MT19937_TEMPERING_MASK_C 0xefc60000
#define MT19937_TEMPERING_SHIFT_U(y) (y >> 11)
#define MT19937_TEMPERING_SHIFT_S(y) (y << 7)
#define MT19937_TEMPERING_SHIFT_T(y) (y << 15)
#define MT19937_TEMPERING_SHIFT_L(y) (y >> 18)

static void mt19937_twist(mt19937_t *mt);

void mt19937_init(mt19937_t *mt, uint32_t seed) {
    mt->state[0] = seed;
    for (int i = 1; i < MT19937_N; i++) {
        mt->state[i] = 0xffffffff & (1812433253 * (mt->state[i-1] ^ (mt->state[i-1] >> 30)) + i);
    }
    mt->index = MT19937_N;
}

float mt19937_float(mt19937_t *mt) {
    return mt19937_float_range(mt, 0.0f, 1.0f);
}

float mt19937_float_range(mt19937_t *mt, float min_val, float max_val) {
    uint32_t y;
    static uint32_t mag01[2] = {0, 0x9908b0df};
    if (mt->index >= MT19937_N) {
        mt19937_twist(mt);
    }
    y = mt->state[mt->index++];
    y ^= MT19937_TEMPERING_SHIFT_U(y);
    y ^= MT19937_TEMPERING_SHIFT_S(y) & MT19937_TEMPERING_MASK_B;
    y ^= MT19937_TEMPERING_SHIFT_T(y) & MT19937_TEMPERING_MASK_C;
    y ^= MT19937_TEMPERING_SHIFT_L(y);
    float val = (float)y / 4294967296.0f; // divide by 2^32
    return min_val + val * (max_val - min_val);
}

static void mt19937_twist(mt19937_t *mt) {
    static uint32_t mag01[2] = {0, 0x9908b0df};
    uint32_t y;
    int i;
    for (i = 0; i < MT19937_N - MT19937_M; i++) {
        y = (mt->state[i] & MT19937_UPPER_MASK) | (mt->state[i+1] & MT19937_LOWER_MASK);
        mt->state[i] = mt->state[i + MT19937_M] ^ (y >> 1) ^ mag01[y & 0x1];
	}
	for (; i < MT19937_N - 1; i++) {
		y = (mt->state[i] & MT19937_UPPER_MASK) | (mt->state[i+1] & MT19937_LOWER_MASK);
		mt->state[i] = mt->state[i + (MT19937_M - MT19937_N)] ^ (y >> 1) ^ mag01[y & 0x1];
	}
	y = (mt->state[MT19937_N - 1] & MT19937_UPPER_MASK) | (mt->state[0] & MT19937_LOWER_MASK);
	mt->state[MT19937_N - 1] = mt->state[MT19937_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];
	mt->index = 0;
}