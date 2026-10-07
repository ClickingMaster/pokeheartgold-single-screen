#pragma force_active on
#include "global.h"
#include "assert.h"

typedef struct Ov49Pair {
    u16 a;
    u16 b;
} Ov49Pair;

Ov49Pair ov42_022282F4(void *obj);
Ov49Pair ov49_02258E34(void **data);

// @fn ov49_02258E34
Ov49Pair ov49_02258E34(void **data) {
    GF_ASSERT(*data != NULL);
    return ov42_022282F4(*data);
}
