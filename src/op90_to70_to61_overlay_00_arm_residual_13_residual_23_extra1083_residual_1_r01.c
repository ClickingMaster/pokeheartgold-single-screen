#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x74];
    s8 key[8];
} KeyObj;
int rand(void);
void ov00_0221446C(KeyObj *obj);
// @fn ov00_0221446C
void ov00_0221446C(KeyObj *obj) {
    int i;
    int parity;
    obj->key[0] = rand() % 93 + 33;
    parity = 0;
    i = 1;
    for (; i < 8; i++) {
        parity = ((obj->key[0] & 1) ^ (parity ^ ((i ^ obj->key[i - 1]) & 1))) ^ (obj->key[0] < 0x4f ? 1 : 0) ^ (obj->key[i - 1] < obj->key[0] ? 1 : 0);
        obj->key[i] = rand() % 93 + 33;
        if ((parity != 0 && (obj->key[i] & 1) == 0) || (parity == 0 && (obj->key[i] & 1) == 1)) {
            obj->key[i]++;
        }
    }
}
