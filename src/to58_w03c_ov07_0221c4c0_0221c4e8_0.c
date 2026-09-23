#include "global.h"
void GF_AssertFail(void);
void *ov07_0221C4C0(u8 *, int);
void *ov07_0221C4E8(u8 *, int);

void *ov07_0221C4C0(u8 *state, int index) {
    if (index >= 10) GF_AssertFail();
    if (state == NULL) GF_AssertFail();
    if (*(void **)(state + 0xDC + index * 4) == NULL) GF_AssertFail();
    return *(void **)(state + 0xDC + index * 4);
}

void *ov07_0221C4E8(u8 *state, int index) {
    if (index >= 5) GF_AssertFail();
    if (state == NULL) GF_AssertFail();
    if (*(void **)(state + 0x13C + index * 4) == NULL) GF_AssertFail();
    return *(void **)(state + 0x13C + index * 4);
}
