#include "to50_overlay65_r14_r2_private.h"

void ov65_0221BFEC(void *work) {
    void *p0 = sub_02034818(sub_0203769C());
    void *p1 = sub_02034818(sub_0203769C() ^ 1);
    void *s0 = PlayerProfile_GetPlayerName_NewString(p0, 0x1a);
    void *s1 = PlayerProfile_GetPlayerName_NewString(p1, 0x1a);
    void *s2 = NewString_ReadMsgData(PTR_AT(work, 0x190), 0x32);
    ov65_0221FB4C((u8 *)work + 0x444, s0, 10, 0, 1, 1);
    ov65_0221FB4C((u8 *)work + 0x454, s1, 10, 0, 1, 1);
    ov65_0221FB4C((u8 *)work + 0x4a4, s2, 5, 0, 1, 1);
    String_Delete(s2);
    String_Delete(s1);
    String_Delete(s0);
}

void ov65_0221C08C(void *work) {
    void *narc = NARC_New(0x34, 0x1a);
    ov65_0221CE98(narc);
    ov65_0221CEB8(PTR_AT(work, 0x180));
    ov65_0221D280(work, narc);
    ov65_0221C1C4(work);
    ov65_0221BFEC(work);
    LoadUserFrameGfx2(PTR_AT(work, 0x180), 0, 0x3d9, 10, (u8)Options_GetFrame(PTR_AT(PTR_AT(work, 8), 0x18)), 0x1a);
    LoadUserFrameGfx1(PTR_AT(work, 0x180), 0, 0x3f7, 11, 0, 0x1a);
    ov65_0221FB90((u8 *)work + 0x594, 15, 1, PTR_AT(work, 0x190), PTR_AT(work, 0x184));
    ToggleBgLayer(4, 1);
    ToggleBgLayer(5, 1);
    ToggleBgLayer(6, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    ToggleBgLayer(0, 1);
    ToggleBgLayer(1, 1);
    ToggleBgLayer(2, 1);
    ToggleBgLayer(3, 1);
    GfGfx_EngineATogglePlanes(0x10, 1);
    ov65_0221D3B8();
    ov65_0221D3E8(work, narc);
    ov65_0221D930(work);
    ov65_0221C46C(PTR_AT(work, 0x2224), 0, work);
    ov65_0221C46C(PTR_AT(work, 0x2228), 6, work);
    Sprite_SetDrawFlag(PTR_AT(work, 0x344), 1);
    Sprite_SetDrawFlag(PTR_AT(work, 0x348), 1);
    Main_SetVBlankIntrCB(ov65_0221CE1C, work);
    NARC_Delete(narc);
}

void ov65_0221C1C4(void *work) {
    int a;
    int b;
    ov65_0221C3DC(PTR_AT(work, 0x184), PTR_AT(work, 0x2224), 0);
    ov65_0221C3DC(PTR_AT(work, 0x184), PTR_AT(work, 0x2228), 6);
    ov65_0221C414((u8 *)work + 0x4b4, PTR_AT(work, 0x184), PTR_AT(work, 0x190), PTR_AT(work, 0x2224), 0);
    ov65_0221C414((u8 *)work + 0x514, PTR_AT(work, 0x184), PTR_AT(work, 0x190), PTR_AT(work, 0x2228), 6);
    a = Party_GetCount(PTR_AT(work, 0x2224));
    b = Party_GetCount(PTR_AT(work, 0x2228));
    ov65_0221D25C(PTR_AT(work, 0x180), a, b);
}

BOOL WirelessTradeSelectMon_Main(OverlayManager *man, int *state) {
    void *work = OverlayManager_GetData(man);
    int ret = 0;
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
            ov65_0221BFEC(work);
        }
        break;
    case 1:
        switch (U32_AT(work, 0x54)) {
        case 0:
            U32_AT(work, 0x54) = ov65_0221C5E0(work);
            break;
        case 1:
            U32_AT(work, 0x54) = ov65_0221CC0C(work);
            break;
        case 2:
            BeginNormalPaletteFade(0, 0, 0, 0, 8, 1, 0x1a);
            *state = 2;
            break;
        case 3:
            BeginNormalPaletteFade(0, 0, 0, 0, 8, 1, 0x1a);
            U32_AT(work, 0x54) = 4;
            break;
        case 4:
            if (IsPaletteFadeFinished()) {
                ov65_0221CD0C(work);
                ov65_0221CCB0(work);
                ov65_0221D1C8(PTR_AT(work, 0x180));
                U32_AT(work, 0x50) = 1;
                U32_AT(work, 0x54) = 5;
                ov65_0221E06C(work, _s32_div_f(U32_AT(work, 0x94), 6));
            }
            break;
        case 5:
            if (OverlayManager_Run(PTR_AT(work, 0x4c))) {
                OverlayManager_Delete(PTR_AT(work, 0x4c));
                ov65_0221C08C(work);
                U32_AT(work, 0x50) = 0;
                U32_AT(work, 0x94) = U8_AT(work, 0x20) + 6 * U32_AT(work, 0x48);
                ov65_0221CB5C(work);
                ov65_0221DD34(U32_AT(work, 0x94), PTR_AT(work, 0x344), 0);
                sub_0203A880();
                U32_AT(work, 0x54) = 6;
            }
            break;
        case 6:
            BeginNormalPaletteFade(0, 1, 1, 0, 8, 1, 0x1a);
            U32_AT(work, 0x54) = 7;
            break;
        case 7:
            if (IsPaletteFadeFinished()) {
                U32_AT(work, 0x54) = 1;
            }
            break;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            ret = 1;
        }
        break;
    }
    if (U32_AT(work, 0x50) == 0) {
        ov65_0221F714(work);
        SpriteList_RenderAndAnimateSprites(PTR_AT(work, 0x1a0));
    }
    sub_020399FC(0x1a, PTR_AT(work, 0x180));
    return ret;
}
