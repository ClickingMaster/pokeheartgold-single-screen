#include "to55_overlay57_ab58_private.h"

#define WSTATE(p) (*(s32 *)((u8 *)(p) + 0x404))
#define WDELAY(p) (*(s32 *)((u8 *)(p) + 0x408))

int ov57_0223AB58(Ov57WorkW11 *w) {
    u32 capsule;
    Ov57BallParams ball;
    u8 seals[24];
    Ov57PicParams pic;
    Ov57BallParams ball2;
    int result;

    switch (WSTATE(w)) {
    case 0:
        GfGfx_EngineATogglePlanes(0x10, TRUE);
        PaletteData_BeginPaletteFade(w->paletteData, 2, 0x80B, 0, 0, 10, 0);
        PaletteData_BeginPaletteFade(w->paletteData, 8, 0xFFFF, 0, 0, 10, 0);
        ov57_0223B948(w, FALSE);
        WDELAY(w) = 0;
        ov57_0223B7C4(w);
        ov57_0223B950(w);
        ov57_0223BB38(w, TRUE);
        ov57_02239728((u8 *)w + 0xEC, 0, 13, 0);
        WSTATE(w)
        ++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(w->paletteData) == 0) {
            ball.kind = 1;
            ball.heapId = 52;
            ball.callbackId = 0;
            ball.ball = 0xFF;
            ball.unk14 = 0;
            ball.unk18 = 1;
            ball.spriteSystem = w->spriteSystem;
            ball.paletteData = w->paletteData;
            ball.species = GetMonData(w->mon, 0x9B, NULL);
            w->ballWork = ov07_02233DB8(&ball);
            WSTATE(w)
            ++;
        }
        break;
    case 2: {
        volatile u32 *p = (u32 *)&pic;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
        pic.kind = 1;
        pic.mon = w->mon;
        capsule = w->capsuleIndex + 1;
        ov57_0223857C(seals, (u8 *)w);
        SetMonData(w->mon, 0xA2, &capsule);
        SetMonData(w->mon, 0xAB, seals);
        w->picWork = ov07_02232694(52, (Ov57PicParams *)&pic);
        ov07_022329B0(w->picWork);
        WSTATE(w)
        ++;
        WDELAY(w) = 0;
        break;
    case 3:
        if (ov07_02232A04(w->picWork) == TRUE && ov07_02233EA0(w->ballWork) == TRUE) {
            ov57_0223BA40(w);
            ov07_02232A44(w->picWork);
            ov57_0223BB38(w, FALSE);
            ov57_0223BA1C(w);
            PlaySE(0x6C5);
            WSTATE(w)
            ++;
        }
        break;
    case 4:
        result = ov57_0223BA40(w);
        if (ov07_02233E88(w->ballWork) == FALSE && result == FALSE) {
            ov07_02233ECC(w->ballWork);
            GfGfx_EngineATogglePlanes(0x10, FALSE);
            ov57_0223B9C8(w);
            WDELAY(w) = 0;
            WSTATE(w)
            ++;
        }
        break;
    case 5:
        if (ov07_02232A54(w->picWork) == FALSE && sub_02017068(w->anim, 0) == TRUE && Pokepic_IsAnimFinished(w->pokepic) == FALSE) {
            ov07_02232AB8(w->picWork);
            WSTATE(w)
            ++;
        }
        break;
    case 6:
        WDELAY(w)
        ++;
        if (WDELAY(w) >= 30) {
            WDELAY(w) = 0;
            ball2.kind = 1;
            ball2.heapId = 52;
            ball2.callbackId = 5;
            ball2.ball = 0xFF;
            ball2.unk14 = 0;
            ball2.unk18 = 1;
            ball2.spriteSystem = w->spriteSystem;
            ball2.paletteData = w->paletteData;
            ball2.species = GetMonData(w->mon, 0x9B, NULL);
            w->ballWork = ov07_02233DB8(&ball2);
            ov57_0223BB5C();
            GfGfx_EngineATogglePlanes(0x10, TRUE);
            WSTATE(w)
            ++;
        }
        break;
    case 7:
        result = ov57_0223BABC(w);
        if (ov07_02233E88(w->ballWork) == FALSE && result == FALSE) {
            PaletteData_BeginPaletteFade(w->paletteData, 2, 0x80B, 0, 10, 0, 0);
            PaletteData_BeginPaletteFade(w->paletteData, 8, 0xFFFF, 0, 10, 0, 0);
            ov57_0223BB38(w, TRUE);
            ov57_0223BB4C(w);
            ov07_02233ECC(w->ballWork);
            WSTATE(w)
            ++;
        }
        break;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(w->paletteData) == 0) {
            GfGfx_EngineATogglePlanes(0x10, FALSE);
            ov57_0223B948(w, TRUE);
            WSTATE(w) = 0;
            w->parentState = 4;
            ov57_02239728((u8 *)w + 0xEC, 0, 0xFFFF, 0);
            return 4;
        }
        break;
    }
    return 5;
}
