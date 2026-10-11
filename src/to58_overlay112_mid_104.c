#include "to58_overlay112_mid_private.h"
int ov112_021ED264(Ov112State *p) {
    MIi_CpuClearFast(0,(u32 *)((u8 *)p+0xaabc),0x6c8);
    MIi_CpuClearFast(0,(u32 *)((u8 *)p+0x9d70),0xd4c);
    PokewalkerApp_LoadTilemap(p,2,0xe);
    PokewalkerApp_PrintExit(p,1);
    ov112_021E7D7C(p);
    ov112_021E74A8((u8 *)p+0x10f4,(u8 *)p+0xc910,(u8 *)p+0xf1d0);
    PokewalkerApp_StartLink(p);
    ov112_021EA5A4(p,2);
    ManagedSprite_SetAnim((void *)W(p,0x1e550),0x19);
    ManagedSprite_SetAnimationFrame((void *)W(p,0x1e550),0);
    ov112_021ED25C(p);
    return 0x19;
}
