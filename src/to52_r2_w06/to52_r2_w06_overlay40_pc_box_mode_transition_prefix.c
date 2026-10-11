#include "private.h"

#define BLEND(p, w, mask) PaletteData_BlendPalettes(PTR(p, 0x28), mask, 12, (u8)U32(w, 8), (u16)U32(p, 0x58))

int ov40_02234A38(void *p) {
    void *w = PTR(p, 0x860);
    switch (S32(p, 8)) {
    case 0:
        ov40_022307DC(p, 0x3c, 7);
        GfGfx_EngineATogglePlanes(8, 1);
        GfGfx_EngineBTogglePlanes(8, 1);
        GfGfx_EngineATogglePlanes(4, 1);
        GfGfx_EngineBTogglePlanes(4, 1);
        PTR(w, 0x1d0) = TouchHitboxController_Create(ov40_02245708, 9, ov40_02235FD0, p, 0x6d);
        S32(p, 8)
        ++;
        break;
    case 1:
        VsRecorder_StepFade((u8 *)w + 8, 0);
        if (VsRecorder_StepBlend(w, (u8 *)w + 4, 0, 0)) {
            ov40_0222DED0(p, 0x72);
            ov40_02235FFC(p);
            S32(p, 8)
            ++;
        }
        BLEND(p, w, 3);
        break;
    case 2:
        TouchHitboxController_IsTriggered(PTR(w, 0x1d0));
        if (TouchscreenHitbox_TouchNewIsIn(ov40_022451C4) || S32(w, 0x1e4) == 1) {
            ov40_02230944(p);
            S32(p, 8)
            ++;
        }
        break;
    case 3:
        ov40_0223610C();
        TouchHitboxController_Destroy(PTR(w, 0x1d0));
        ov40_0222DFB0(p);
        S32(p, 8)
        ++;
        /* fallthrough */
    case 4:
        if (S32(w, 0x1e4) == 1) {
            if (VsRecorder_StepBlend(w, (u8 *)w + 4, 1, 2)) {
                S32(p, 8)
                ++;
            }
        } else {
            VsRecorder_StepFade((u8 *)w + 8, 1);
            if (VsRecorder_StepBlend(w, (u8 *)w + 4, 1, 2)) {
                S32(p, 8)
                ++;
            }
            BLEND(p, w, 3);
        }
        break;
    default:
        if (S32(w, 0x1e4) == 1) {
            VsRecorder_SetSubState(p, 4);
        } else {
            VsRecorder_SetSubState(p, 5);
        }
        break;
    }
    return 0;
}

int ov40_02234BBC(void *p) {
    void *w = PTR(p, 0x860);
    switch (S32(p, 8)) {
    case 0:
        ov40_022359B4(p, 1);
        ov40_022307DC(p, 0x3a, 7);
        S32(p, 8)
        ++;
        break;
    case 1:
        if (VsRecorder_StepBlend(w, (u8 *)w + 4, 0, 2)) {
            ov40_0222DED0(p, 0x72);
            ov40_02230964(p, 1);
            ov40_0222F9D4((u8 *)p + 0x47c, p);
            *(Block44 *)((u8 *)w + 0x1ec) = ov40_022451F4;
            U32(w, 0x1ec) = U32(w, 0x1e8);
            U32(w, 0x1f0) = U32(w, 0x1d4);
            ov40_0222F734((u8 *)p + 0x49c);
            ov40_0222E9B8((u8 *)p + 0x49c, p, PTR(w, 0x1e0), (u8 *)w + 0x1ec);
            ov40_0222FA5C((u8 *)p + 0x47c, (u8 *)p + 0x49c);
            ov40_0222F740((u8 *)p + 0x49c, p, 2);
            ov40_02230964(p, 0);
            S32(w, 0x2e0) = 0;
            S32(p, 8)
            ++;
        }
        break;
    case 2: {
        int ret;
        ov40_0222FA88((u8 *)p + 0x47c);
        ov40_0222F6D0((u8 *)p + 0x49c, S16(p, 0x48c));
        ret = ov40_0222F38C((u8 *)p + 0x49c, p);
        if (ret != 0) {
            S32(w, 0x2e0) = ret;
            ov40_02230944(p);
            S32(p, 8)
            ++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(ov40_022451C4)) {
            ov40_02230944(p);
            S32(p, 8)
            ++;
        }
        break;
    }
    case 3:
        ov40_0222DFB0(p);
        ov40_02230964(p, 1);
        ov40_0222FA24((u8 *)p + 0x47c);
        ov40_0222F720((u8 *)p + 0x49c);
        ov40_0222F920((u8 *)p + 0x49c, p);
        ov40_02230964(p, 0);
        S32(p, 8)
        ++;
        /* fallthrough */
    case 4:
        VsRecorder_StepFade((u8 *)w + 8, 1);
        if (VsRecorder_StepBlend(w, (u8 *)w + 4, 1, 0)) {
            S32(p, 8)
            ++;
        }
        BLEND(p, w, 3);
        break;
    default:
        ov40_02235FA0(p);
        if (S32(w, 0x2e0) != 0) {
            VsRecorder_SetSubState(p, 6);
        } else {
            VsRecorder_SetSubState(p, 3);
        }
        break;
    }
    return 0;
}
