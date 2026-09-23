#include "to58_overlay112_mid_private.h"
void ov112_021EA51C(Ov112State *p, int i) {
    int o = i * 4;
    void *sprite = (void *)W(p,0x1e530);
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprite,ov112_021FECE4[o],ov112_021FECE5[o],1<<20);
    ManagedSprite_SetAnim((void *)W(p,0x1e530),ov112_021FECE6[o]);
    ManagedSprite_SetPaletteOverride((void *)W(p,0x1e530),ov112_021FECE7[o]);
    ManagedSprite_SetDrawFlag((void *)W(p,0x1e530),1);
}
