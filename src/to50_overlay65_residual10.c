#include "to50_overlay65_residual10_private.h"

void ov65_0221ED94(void *sprite0, void *sprite1, void *sprite2, s32 step, const Ov65Interp *interp, const Ov65AnimFlags *flags) {
    VecFx32 pos;
    VecFx32 pos1;
    VecFx32 pos2;
    pos.x = interp->x0 + ((interp->x1 - interp->x0) / 20) * step;
    pos.y = interp->y0 + ((interp->y1 - interp->y0) / 20) * step;
    Sprite_SetMatrix(sprite0, &pos);
    if (flags->flag2 != 0) {
        pos1.x = pos.x + 0x14000;
        pos1.y = pos.y + 0x16000;
        Sprite_SetMatrix(sprite1, &pos1);
    }
    if (flags->flagC != 0) {
        pos2.x = pos.x + 0x1c000;
        pos2.y = pos.y + 0x16000;
        Sprite_SetMatrix(sprite2, &pos2);
    }
}

int ov65_0221EE18(void *p) {
    u32 left;
    u32 right;
    U32(p, 0x22c8)
    ++;
    left = U32(p, 0x94);
    ov65_0221ED94(PTR(p, 0x37c + left * 4), PTR(p, 0x3ac + left * 4), PTR(p, 0x3dc + left * 4), U32(p, 0x22c8), (Ov65Interp *)((u8 *)p + 0x22d0), (Ov65AnimFlags *)((u8 *)p + 0x69c + left * 16));
    right = U32(p, 0x98);
    ov65_0221ED94(PTR(p, 0x37c + right * 4), PTR(p, 0x3ac + right * 4), PTR(p, 0x3dc + right * 4), U32(p, 0x22c8), (Ov65Interp *)((u8 *)p + 0x22e8), (Ov65AnimFlags *)((u8 *)p + 0x69c + right * 16));
    if (U32(p, 0x22c8) == 21) {
        ov65_0221D5FC((u8 *)p + 0x574, U32(p, 0x2224), U32(p, 0x94), 8, 1);
        ov65_0221D5FC((u8 *)p + 0x584, U32(p, 0x2228), U32(p, 0x98) - 6, 8, 1);
        ov65_0221CADC(p, U32(p, 0x98));
        FN(p, 0x2220) = (void (*)(void *))ov65_0221F028;
        Sprite_SetDrawFlag(PTR(p, 0x424), 1);
    }
    return 0;
}

void ov65_0221EF0C(void *p) {
    s32 i;
    u8 *rec;
    const Ov65Point *pt;
    u8 *spriteWalk;
    void *windows;
    ClearWindowTilemapAndScheduleTransfer((u8 *)p + 0x574);
    ClearWindowTilemapAndScheduleTransfer((u8 *)p + 0x584);
    rec = p;
    pt = ov65_0221FF4C;
    i = 0;
    spriteWalk = p;
    windows = (u8 *)p + 0x444;
    for (; i < 12; i++, rec += 16, pt++, spriteWalk += 4) {
        if (U16(rec, 0x69c) != 0) {
            ov65_0221F748(PTR(spriteWalk, 0x37c), pt->x + 16, pt->y - 6);
            Sprite_SetDrawFlag(PTR(spriteWalk, 0x37c), 1);
            if (U16(rec, 0x69e) != 0) {
                ov65_0221F748(PTR(spriteWalk, 0x3ac), pt->x + 36, pt->y + 16);
                Sprite_SetDrawFlag(PTR(spriteWalk, 0x3ac), 1);
            }
            if (PTR(rec, 0x6a8) != NULL) {
                ov65_0221F748(PTR(spriteWalk, 0x3dc), pt->x + 44, pt->y + 16);
                Sprite_SetDrawFlag(PTR(spriteWalk, 0x3dc), 1);
            }
            ScheduleWindowCopyToVram((u8 *)windows + (i + 7) * 16);
        }
    }
    Sprite_SetDrawFlag(PTR(p, 0x344), 1);
    Sprite_SetDrawFlag(PTR(p, 0x348), 1);
    ov65_0221D8C4((u8 *)p + 0x444, 1, p);
    Sprite_SetDrawFlag(PTR(p, 0x418), 0);
    Sprite_SetDrawFlag(PTR(p, 0x424), 0);
}

int ov65_0221F028(void *p) {
    void *mon;
    FillBgTilemapRect(PTR(p, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
    mon = Party_GetMonByIndex(PTR(p, 0x2224), U32(p, 0x94));
    BufferBoxMonNickname(PTR(p, 0x188), 0, Mon_GetBoxMon(mon));
    mon = Party_GetMonByIndex(PTR(p, 0x2228), U32(p, 0x98) - 6);
    BufferBoxMonNickname(PTR(p, 0x188), 1, Mon_GetBoxMon(mon));
    ov65_0221FB90((u8 *)p + 0x5b4, 21, 1, U32(p, 0x190), PTR(p, 0x188));
    FN(p, 0x2220) = ov65_0221F120;
    return 0;
}
