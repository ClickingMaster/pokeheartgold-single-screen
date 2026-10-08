#pragma force_active on
#include "global.h"
void MI_CpuCopy8(const void *src, void *dest, u32 size);
u16 ov98_0221EFA4(u16, u32);
BOOL ov98_0221EFB4(u32, u32, u16);
typedef struct { u16 a, b, c; } Meta;
typedef struct { u16 id; Meta meta; } Record;
typedef struct { Record recs[5]; u8 extra[4]; } Group;
typedef struct { u16 pad[2]; Meta meta; } DefaultData;
extern const DefaultData ov99_021EA350;
typedef struct { u8 pad[0xbc]; u32 flags[10]; } Owner;
void ov99_021E92EC(Owner *owner, Group *src, Group *dst);
// @fn ov99_021E92EC
void ov99_021E92EC(Owner *owner, Group *src, Group *dst) {
    int k, i, j;
    Meta tmp;
    Meta def;
    MI_CpuCopy8(src, dst, sizeof(Group) * 10);
    k = 0;
    def = ov99_021EA350.meta;
    for (; k < 10; k++) {
        for (i = 0; i < 5; i++) {
            if (!ov98_0221EFB4((u8)k, 0, src[k].recs[i].id)) {
                break;
            }
        }
        if (i == 0) {
            owner->flags[k] = 1;
        }
        for (j = 0; j < 5; j++) {
            if (j == i) {
                tmp = def;
                MI_CpuCopy8(&tmp, &dst[k].recs[j].meta, sizeof(Meta));
                dst[k].recs[j].id = ov98_0221EFA4(k, 0);
            } else if (j > i) {
                dst[k].recs[j] = src[k].recs[j - 1];
            }
        }
    }
}
