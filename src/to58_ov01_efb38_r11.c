#include "field_overlay_effects_internal.h"
BOOL ov01_021F08C0(void *);
BOOL ov01_021F08C0(void *arg) {
    u8 *p = arg;
    if (p[0x2E] == 0) {
        return TRUE;
    }
    u32 width = p[0x2C];
    s16 left = *(s32 *)p - (width >> 1);
    s16 right = left + width;
    u32 height = p[0x2D];
    s16 top = *(s32 *)(p + 0x14) - (height >> 1);
    s16 bottom = top + height;
    ov01_021F0960(*(void **)(p + 0x28), top, bottom, left, right, 0);
    BOOL done = EncounterEffectInterp_Update((Interp *)p);
    EncounterEffectInterp_Update((Interp *)(p + 0x14));
    width = p[0x2C];
    left = *(s32 *)p - (width >> 1);
    right = left + width;
    height = p[0x2D];
    top = *(s32 *)(p + 0x14) - (height >> 1);
    bottom = top + height;
    ov01_021F0960(*(void **)(p + 0x28), top, bottom, left, right, p[0x2F]);
    return done;
}
