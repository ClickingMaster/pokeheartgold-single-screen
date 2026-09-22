#include "global.h"
#pragma require_prototypes off

extern void ov112_021F1F54(void *, int, int, int);
extern void ov112_021F1F3C(void *, int);
extern int ov112_021F0F48(void *, int);

typedef struct Ov112Record {
    u8 pad00[0xA];
    u16 value0A;
    u16 value0C;
    u8 pad0E[0x77];
    u8 value85_0 : 5;
    u8 value85_1 : 2;
    u8 value85_2 : 1;
    u8 value86_0 : 5;
    u8 value86_1 : 2;
    u8 value86_2 : 1;
} Ov112Record;

typedef struct Ov112SpriteSlot Ov112SpriteSlot;
extern void ov112_021F2098(Ov112SpriteSlot *, u16, u8, u8, int, int);

void ov112_021F2204(void *arg0, int arg1, int arg2) {
    u8 *p = arg0;
    Ov112Record *entry = *(Ov112Record **)(p + 0xC0 + p[0x13D] * 4);
    if (arg1 == 0) {
        ov112_021F1F54(p + 0x98, 0x68, 0x48, 0);
        ov112_021F1F3C(p + 0x98, 3);
    } else {
        ov112_021F1F54(p + 0xAC, 0x98, 0x48, 0);
        ov112_021F1F3C(p + 0xAC, 1);
    }
    if (arg2 != 0) {
        ov112_021F2098((Ov112SpriteSlot *)(p + 0x98 + 0x14 * arg1), entry->value0A, entry->value85_0, entry->value85_1, ov112_021F0F48(entry, 1), *(int *)(p + 4));
        return;
    }
    ov112_021F2098((Ov112SpriteSlot *)(p + 0x98 + 0x14 * arg1), entry->value0C, entry->value86_0, entry->value86_1, ov112_021F0F48(entry, 0), *(int *)(p + 4));
}
