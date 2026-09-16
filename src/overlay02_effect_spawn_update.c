#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224DF1C(void *work) {
    VecFx32 pos1;
    VecFx32 pos2;
    int i;
    *(fx32 *)((u8 *)work + 0xe80) = 0x1000;
    *(fx32 *)((u8 *)work + 0xe84) = 0x3000;
    PlayerAvatar_CopyPositionVector(((FieldSystem *)*(void **)((u8 *)work + 0xe78))->playerAvatar, &pos1);
    if (*(u16 *)((u8 *)work + 0xe9a) != 0) {
        MapObject_CopyPositionVector(FollowMon_GetMapObject(*(FieldSystem **)((u8 *)work + 0xe78)), &pos2);
        {
            u8 *p;
            for (i = 0, p = (u8 *)work; i < 0x12; i++, p += 0xcc) {
                if (*(int *)(p + 0xd8) == 0) {
                    ov02_0224DE10((Field3dObject *)((u8 *)work + 0x10 + i * 0xcc), &pos1, *(fx32 *)((u8 *)work + 0xe80), *(fx32 *)((u8 *)work + 0xe84));
                    break;
                }
            }
        }
        if (i == 0x12) {
            GF_AssertFail();
        }
        {
            u8 *p;
            for (i = 0, p = (u8 *)work; i < 0x12; i++, p += 0xcc) {
                if (*(int *)(p + 0xd8) == 0) {
                    ov02_0224DE10((Field3dObject *)((u8 *)work + 0x10 + i * 0xcc), &pos2, *(fx32 *)((u8 *)work + 0xe80), *(fx32 *)((u8 *)work + 0xe84));
                    return;
                }
            }
        }
    } else {
        u8 *p;
        for (i = 0, p = (u8 *)work; i < 0x12; i++, p += 0xcc) {
            if (*(int *)(p + 0xd8) == 0) {
                ov02_0224DE10((Field3dObject *)((u8 *)work + 0x10 + i * 0xcc), &pos1, *(fx32 *)((u8 *)work + 0xe80), *(fx32 *)((u8 *)work + 0xe84));
                return;
            }
        }
    }
    GF_AssertFail();
}
