#include "field_overlay_effects_internal.h"

void ov01_021F0E74(void) {
    u8 *p = *(u8 **)sEncounterEffectState;
    if (!p) {
        GF_ASSERT(FALSE);
    }
    (*(u8 **)sEncounterEffectState)[3] = 1;
}

u8 ov01_021F0E90(void) {
    u8 *p = *(u8 **)sEncounterEffectState;
    if (!p) {
        GF_ASSERT(FALSE);
    }
    return (*(u8 **)sEncounterEffectState)[2];
}

void ov01_021F0EAC(void) {
    void *p = *(void **)sEncounterEffectState;
    if (p) {
        ParticleSystem_DrawAll(p);
    }
}

void ov01_021F0EC0(void) {
    u8 *p = *(u8 **)sEncounterEffectState;
    if (!p) {
        GF_ASSERT(FALSE);
    }
    ParticleSystem_FreeAll();
    ov01_021F1210(*(void **)(*(u8 **)sEncounterEffectState + 4));
    ov01_021E631C(*(void **)(*(u8 **)sEncounterEffectState + 4), 1);
    ov01_021E6340(*(void **)(*(u8 **)sEncounterEffectState + 4), 1);
    Heap_Free(*(void **)sEncounterEffectState);
    *(void **)sEncounterEffectState = NULL;
}
