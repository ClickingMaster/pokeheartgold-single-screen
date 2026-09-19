#include "overlay27_field_ui_state_r6_internal.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

typedef struct Ov27ModeBitsR6 {
    u32 bit0 : 1;
    u32 mode : 4;
    u32 rest : 27;
} Ov27ModeBitsR6;

typedef struct Ov27FlagsR6 {
    u8 pad[0xd2];
    u8 low : 6;
    u8 high : 2;
} Ov27FlagsR6;

#define MODE(p)      (((Ov27ModeBitsR6 *)((u8 *)(p) + 0x51c))->mode)
#define LOW_FLAGS(p) (((Ov27FlagsR6 *)(p))->low)

BOOL ov27_0225A89C(void *work) {
    if (!IsPaletteFadeFinished() || PTR(work, 0x6c) == NULL || LOW_FLAGS(work) != 0 || !FieldSystem_IsPlayerMovementAllowed(work) || ov27_0225BD44(work) || MapSceneScriptCheck(work) || sub_02058AA0()) {
        return TRUE;
    }
    return FALSE;
}

void ov27_0225A8E8(void *work, BOOL enable) {
    if (enable) {
        if (MODE(work) == 5 && LOW_FLAGS(PTR(work, 0x10)) == 0) {
            ov27_0225B398(work, -1);
        }
        Sprite_SetOamMode(PTR(work, 0x3bc), 1);
        Sprite_SetOamMode(PTR(work, 0x3c4), 1);
        Sprite_SetOamMode(PTR(work, 0x3ac), 1);
        Sprite_SetOamMode(PTR(work, 0x3b4), 1);
        Sprite_SetOamMode(PTR(work, 0x3b0), 1);
        Sprite_SetOamMode(PTR(work, 0x3b8), 1);
        if (LOW_FLAGS(PTR(work, 0x10)) != 0) {
            ov27_0225B4AC((u8 *)work + 0x390, U32(work, 0x14));
        } else {
            ov27_0225B4AC((u8 *)work + 0x390, -1);
        }
        if (MODE(work) == 2) {
            Sprite_SetOamMode(PTR(work, 0x3a8), 0);
        }
        G2x_SetBlendAlpha_(0x04001050, 0, 0x23, 6, 9);
    } else {
        *(vu16 *)0x04001050 = 0;
    }
}

void ov27_0225A9C0(void *work, BOOL enable) {
    Bag *bag;
    BOOL has1;
    BOOL has2;
    if (!ov27_0225BDAC(work)) {
        enable = FALSE;
    }
    if (enable) {
        bag = Save_Bag_Get(PTR(PTR(work, 0x10), 0xc));
        has1 = Bag_GetRegisteredItem1(bag) != 0;
        has2 = Bag_GetRegisteredItem2(bag) != 0;
        Sprite_SetDrawFlag(PTR(work, 0x3ac), has1);
        Sprite_SetDrawFlag(PTR(work, 0x3b4), has1);
        Sprite_SetDrawFlag(PTR(work, 0x3b0), has2);
        Sprite_SetDrawFlag(PTR(work, 0x3b8), has2);
    } else {
        Sprite_SetDrawFlag(PTR(work, 0x3ac), FALSE);
        Sprite_SetDrawFlag(PTR(work, 0x3b4), FALSE);
        Sprite_SetDrawFlag(PTR(work, 0x3b0), FALSE);
        Sprite_SetDrawFlag(PTR(work, 0x3b8), FALSE);
    }
}
