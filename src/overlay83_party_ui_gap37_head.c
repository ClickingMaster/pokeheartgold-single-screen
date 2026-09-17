#include "overlay83_party_ui_gap37_internal.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov83_022459AC(u8 *work) {
    int count = ov80_02237B24(work[9], 1);
    int i = 0;
    u8 *cursor;
    if (count > 0) {
        cursor = work;
        do {
            void *mon = Party_GetMonByIndex(PTR(work, 0x55c), i);
            u32 a = GetMonData(mon, 0xa3, NULL);
            u32 b = GetMonData(mon, 0xa4, NULL);
            int v = ov80_0222A43C((u16)a, (u16)b);
            if (PTR(cursor, 0x4e4) != NULL) {
                ov83_02247600(PTR(cursor, 0x4e4), v);
                if (i != ov83_02247768(work[0x14], work[0xd]) || work[0xd] >= work[0x15]) {
                    ov83_0224760C(PTR(cursor, 0x4e4), 0);
                } else {
                    ov83_0224760C(PTR(cursor, 0x4e4), 1);
                }
            }
            i++;
            cursor += 4;
        } while (i < count);
    }
}

int ov83_02245A40(u8 *work, int value, int arg) {
    int statId = sub_0205C1F0(work[9]);
    int stat = sub_0205C268(sub_0205C1F0(work[9]));
    if (FrontierSave_GetStat(PTR(work, 4), statId, stat) < (u32)value) {
        ov83_022453C0(work);
        work[0xa] = ov83_022448AC(work, arg, 1);
        work[8] = 0x10;
        return 0;
    }
    if (ov80_02237D8C(work[9]) == 0) {
        ov83_02245094(work + 0xc0);
        ov80_02237FA4(PTR(work, 4), work[9], value);
        ov83_022453DC(work, work + 0x50);
        if (value == 2) {
            ov83_02245288(work, work[0xd]);
        } else {
            ov83_02245318(work, work[0xd]);
        }
        work[8] = 0xd;
        return 0;
    }
    work[0x10] = 1;
    return 1;
}
