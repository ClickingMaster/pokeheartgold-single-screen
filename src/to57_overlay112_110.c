#include "to57_overlay112_110_private.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

int ov112_021ED66C(void *p) {
    struct Flags {
        u8 pad[0x10E7];
        u8 unused : 2;
        u8 flag : 1;
    };
    int state;

    PokewalkerIr_GetChunkProgress();
    state = PokewalkerIr_GetTotalProgress();
    if (state != 0) {
        PokewalkerApp_SpawnFallingParticle(p);
    }
    ov112_021EA60C(p);
    ov112_021ED314(state);

    state = PokewalkerIr_GetAndClearError();
    switch (state) {
    case 2:
        if (U16_AT(p, 0x16) == 1) {
            U16_AT(p, 0x14) = 0x2A;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
        U16_AT(p, 0x14) = state;
        break;
    case 15:
        ManagedSprite_SetDrawFlag(PTR_AT(p, 0x1E538), FALSE);
        if (ov112_021ED610(p) && ((struct Flags *)p)->flag == 1) {
            GetSpeciesNameIntoArray(U16_AT(p, 0x9D44), 0x9A, (u16 *)((u8 *)p + 0x9D54));
        }
        ov112_021EAA98(p, (u8 *)p + 0x1F2C0, 1);
        ov112_021ED530(p);
        ov112_021ED590(p);
        ov112_021ED7DC(p);
        U32_AT(p, 0x1EC7C) = U32_AT(p, 0x10F0) - U32_AT(p, 0x1D758);
        return 0x25;
    }

    if (U16_AT(p, 0x14) != 0) {
        ManagedSprite_SetDrawFlag(PTR_AT(p, 0x1E538), FALSE);
        U32_AT(p, 0xC) = 0x3C;
        PokewalkerIr_Stop();
        return 0x20;
    }
    if (!ov112_021E7668()) {
        PokewalkerApp_ShowSubBg1(p, 1);
        if (PokewalkerApp_GetTouchedButton(5) == 0 || (U32_AT(gSystem, 0x48) & 1) || (U32_AT(gSystem, 0x48) & 2)) {
            U16_AT(p, 0x14) = 0x21;
            U32_AT(p, 0xC) = 0x42;
            PokewalkerIr_Stop();
            ov112_021EC440(p, 3, 3, 0x21);
            PlaySE(0x5DD);
            return 0x41;
        }
    } else {
        if (U16_AT(p, 0x16) == 0) {
            PokewalkerApp_ShowMessageBox(p, 2, 6);
            U16_AT(p, 0x16) = 1;
        }
        PokewalkerApp_ShowSubBg1(p, 0);
    }
    return 0x1B;
}
