#include "global.h"

#include "overlay_manager.h"
#include "screen_fade.h"
#include "sound.h"

extern const OverlayManagerTemplate ov112_021FF124;
extern void PokewalkerApp_Teardown(void *);
extern int ov112_021F31D8(void *);
int ov112_021EF568(void *);
int ov112_021EF5AC(void *);

#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "string_util.h"

typedef struct Ov112BoxMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 unkC;
    u16 unkE;
    u16 isShiny;
    u16 gender;
    u16 unk14;
    u16 unk16;
    u16 nickname[12];
} Ov112BoxMon;

typedef struct Ov112Selection {
    u8 pad00[0xC];
    u32 box;
    u32 slot;
    u32 blocked;
    u16 species;
    u16 nickname[11];
    u8 gender;
    u8 shiny;
    u8 markings;
} Ov112Selection;

extern BOOL ov112_021EF1F0(PCStorage *, u32, u32, Ov112BoxMon *);
void ov112_021EF31C(Ov112Selection *, void *);

typedef struct Ov112DisplayMon {
    u32 personality;
    u16 species;
    u8 shiny;
    u8 form;
    u8 gender;
    u8 state;
    u16 nickname[11];
    u8 flags[6];
    u16 unk26;
    u8 unk28;
} Ov112DisplayMon;

typedef struct Ov112BoxWork {
    u8 pad00[8];
    void *display;
} Ov112BoxWork;

extern void ov112_021F04DC(void *, Ov112DisplayMon *);
void ov112_021EF3F8(int, int, void *);

void ov112_021EF31C(Ov112Selection *sel, void *p) {
    if (sel->blocked == 0) {
        u32 box = sel->box;
        if (box == 0x12) {
            Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(*(SaveData **)((u8 *)p + 0x20)), sel->slot);
            sel->species = GetMonData(mon, 5, NULL);
            sel->gender = GetMonData(mon, 0x70, NULL);
            GetMonData(mon, 0x75, sel->nickname);
            sel->shiny = MonIsShiny(mon);
            sel->markings = GetMonData(mon, 0x6F, NULL);
            *(Pokemon **)((u8 *)p + 0x1E42C) = mon;
            *(u32 *)((u8 *)p + 0x1E430) = 0;
        } else {
            PCStorage *storage = SaveArray_PCStorage_Get(*(SaveData **)((u8 *)p + 0x20));
            BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, box, sel->slot);
            Ov112BoxMon temp;
            if (GetBoxMonData(boxMon, 0xAC, NULL) != 0) {
                *(BoxPokemon **)((u8 *)p + 0x1E430) = boxMon;
                *(u32 *)((u8 *)p + 0x1E42C) = 0;
            }
            storage = SaveArray_PCStorage_Get(*(SaveData **)((u8 *)p + 0x20));
            ov112_021EF1F0(storage, sel->box, sel->slot, &temp);
            sel->species = temp.species;
            sel->gender = temp.form;
            CopyU16StringArrayN(sel->nickname, temp.nickname, 11);
            sel->shiny = temp.isShiny;
            sel->markings = temp.gender;
        }
    }
}

void ov112_021EF3F8(int box, int slot, void *p) {
    Ov112BoxWork *work = (Ov112BoxWork *)((u8 *)p + 0x1D764);
    Ov112DisplayMon out;

    if (box == -1 || slot == -1) {
        goto invalid;
    }

    out.state = 2;
    if (box == 0x12) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(*(SaveData **)((u8 *)p + 0x20)), slot);
        u8 i;
        s32 flags;
        out.personality = GetMonData(mon, 0, NULL);
        out.species = GetMonData(mon, 5, NULL);
        out.form = GetMonData(mon, 0x70, NULL);
        GetMonData(mon, 0x75, out.nickname);
        out.shiny = MonIsShiny(mon);
        out.gender = GetMonData(mon, 0x6F, NULL);
        flags = GetMonData(mon, 0xB, NULL);
        for (i = 0; i < 6; i++) {
            if ((flags >> i) & 1) {
                out.flags[i] = 1;
            } else {
                out.flags[i] = 0;
            }
        }
        out.unk26 = GetMonData(mon, 6, NULL);
        out.unk28 = GetMonData(mon, 0xA1, NULL);
        ov112_021F04DC(work->display, &out);
        return;
    } else {
        s32 boxFlags;
        u8 i;
        Ov112BoxMon temp;
        ov112_021EF1F0(SaveArray_PCStorage_Get(*(SaveData **)((u8 *)p + 0x20)), box, slot, &temp);
        out.personality = temp.personality;
        out.species = temp.species;
        out.form = temp.form;
        CopyU16StringArrayN(out.nickname, temp.nickname, 11);
        out.shiny = temp.isShiny;
        out.gender = temp.gender;
        boxFlags = temp.unk16;
        for (i = 0; i < 6; i++) {
            if ((boxFlags >> i) & 1) {
                out.flags[i] = 1;
            } else {
                out.flags[i] = 0;
            }
        }
        out.unk26 = temp.unkC;
        out.unk28 = temp.unk14;
        ov112_021F04DC(work->display, &out);
        return;
    }

invalid:
    out.state = 0;
    ov112_021F04DC(work->display, &out);
}

int ov112_021EF568(void *p) {
    *(void **)((u8 *)p + 0x1D750) = *(void **)((u8 *)p + 0x20);
    *(void **)((u8 *)p + 0x1D754) = (u8 *)p + 0x9DFC;
    BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)0, (enum FadeType)0, 0x7FFF, 6, 1, (enum HeapID)0x9A);
    GF_SndStartFadeOutBGM(0, 0x10);
    return 2;
}

int ov112_021EF5AC(void *p) {
    switch (*(u32 *)((u8 *)p + 8)) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            PokewalkerApp_Teardown(p);
            if (ov112_021F31D8((u8 *)p + 0x9DFC)) {
                return 3;
            }
            *(u32 *)((u8 *)p + 8) = 1;
        }
        break;
    case 1:
        *(OverlayManager **)((u8 *)p + 0x1C) = OverlayManager_New(&ov112_021FF124, (u8 *)p + 0x1D750, (enum HeapID)0x9A);
        *(u32 *)((u8 *)p + 8) = 2;
        break;
    case 2:
        if (OverlayManager_Run(*(OverlayManager **)((u8 *)p + 0x1C))) {
            OverlayManager_Delete(*(OverlayManager **)((u8 *)p + 0x1C));
            *(u32 *)((u8 *)p + 8) = 3;
        }
        break;
    case 3:
        return 3;
    }
    return 2;
}
