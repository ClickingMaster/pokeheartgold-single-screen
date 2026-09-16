#include "global.h"

typedef struct AnimSub {
    u8 unk0;
    u8 mainState;
    s16 data[7];
} AnimSub;

typedef struct AnimEntry {
    u32 active;
    u8 pad04[8];
    u16 unkC;
    u8 pad0E[0x18];
    AnimSub sub;
    u8 pad36[2];
    u32 unk38;
} AnimEntry;

extern void *sub_0209680C(void *a0);
extern void ov80_0222EFD0(void *out, u32 a, u16 b, s16 c);
extern void ov42_022299C0(void *s, void *out);
int ov80_02239C80(void *a0, AnimEntry *entry);

int ov80_02239C80(void *a0, AnimEntry *entry) {
    AnimSub *st = &entry->sub;
    void *unkObj = sub_0209680C(a0);
    u8 buf[8];
    int newState;
    if (st->data[2] > 0) {
        st->data[2]--;
        return 0;
    }
    switch (st->mainState) {
    case 0:
        switch (st->data[0]) {
        case 0:
        case 1:
            if (st->data[1] == 0) {
                st->data[3] = 2;
                st->data[4] = 3;
            } else {
                st->data[3] = 3;
                st->data[4] = 2;
            }
            break;
        case 2:
        case 3:
            if (st->data[1] == 0) {
                st->data[3] = 0;
                st->data[4] = 1;
            } else {
                st->data[3] = 1;
                st->data[4] = 0;
            }
            break;
        default:
            GF_AssertFail();
            return 1;
        }
        st->data[5] = st->data[0];
        st->mainState++;
    case 1:
    case 2:
    case 3:
        ov80_0222EFD0(buf, entry->active, entry->unkC, st->data[st->mainState + 2]);
        ov42_022299C0(*(void **)((u8 *)unkObj + 0x30), buf);
        if (st->data[0] == st->data[st->mainState + 2]) {
            st->data[2] = 0x2d;
            newState = 1;
        } else {
            st->data[2] = 0x1e;
            newState = st->mainState + 1;
        }
        break;
    default:
        return 1;
    }
    st->mainState = newState;
    return 0;
}
