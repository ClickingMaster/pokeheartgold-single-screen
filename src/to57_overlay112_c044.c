#include "global.h"

#include "bg_window.h"
#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save_arrays.h"
#include "sound.h"
#include "sprite_system.h"

#pragma require_prototypes off

#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern int PokewalkerIr_GetAndClearError(void);
extern void PokewalkerIr_Stop(void);
extern BOOL ov112_021E7668(void);
extern void ov112_021E98E8(void *, int);
extern int ov112_021E9888(int);
extern void ov112_021EC440(void *, int, int, int);
extern void *ov112_021EA08C(void *, int, int);
extern void ov112_021EA570(void *);

void ov112_021EC134(void *);

int ov112_021EC044(void *work) {
    int result = 0x11;
    switch (PokewalkerIr_GetAndClearError()) {
    case 2:
        result = 0x12;
        break;
    case 4:
        result = 0x12;
        break;
    case 5:
        result = 0x12;
        break;
    case 7:
        result = 0x12;
        break;
    case 8:
        result = 0x12;
        break;
    case 9:
        result = 0x12;
        break;
    case 11:
        result = 0x12;
        break;
    case 12:
        result = 0x12;
        break;
    case 15:
        ManagedSprite_SetDrawFlag(PTR(work, 0x1E538), FALSE);
        ov112_021EC134(work);
        sub_02032624(PTR(work, 0x1E440));
        U32(work, 0x1D758) = 0;
        U32(work, 0x1D75C) = 0;
        PokewalkerIr_Stop();
        return 0x14;
    }
    if (result != 0x11) {
        PokewalkerIr_Stop();
    }
    if (!ov112_021E7668()) {
        ov112_021E98E8(work, 1);
        if (ov112_021E9888(5) == 0 || (gSystem.newKeys & 1) || (gSystem.newKeys & 2)) {
            PokewalkerIr_Stop();
            ov112_021EC440(work, 1, 3, 0x12);
            PlaySE(0x5DC);
            return 0x21;
        }
    } else {
        ov112_021E98E8(work, 0);
    }
    return result;
}

void ov112_021EC134(void *work) {
    u16 unk0;
    u16 unk2;
    int box = -1;
    int slot = 0;
    PCStorage *storage = SaveArray_PCStorage_Get(PTR(work, 0x20));
    BoxPokemon *boxMon = (BoxPokemon *)((u8 *)work + 0x1F2E8);

    sub_02032688(PTR(work, 0x1E440), &unk0, &unk2);
    if (Pokewalker_TryGetBoxMon(PTR(work, 0x1E440), boxMon)) {
        box = unk2;
        PCStorage_FindFirstEmptySlot(storage, &box, &slot);
        PCStorage_PlaceMonInBoxByIndexPair(storage, box, slot, boxMon);
        U16(work, 0x1F370) = box;
    } else {
        S16(work, 0x1F370) = -1;
    }
}

int ov112_021EC1A8(void *work) {
    PTR(work, 0x1E524) = ov112_021EA08C(work, 0, 0x21);
    ov112_021EA570(work);
    FillBgTilemapRect(PTR(work, 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
    return 0x13;
}
