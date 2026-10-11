#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "nnsys.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "oam_manager.h"

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

extern const GraphicsBanks ov112_021FF0FC;
extern const GraphicsModes ov112_021FF0DC;
extern const ObjCharTransferTemplate ov112_021FF0EC;

extern void ov112_021EA688(void *, int);
extern void PokewalkerApp_Teardown(void *);
extern void *ov112_021F039C(int);
extern void ov112_021F03BC(void *, void *);
extern void ov112_021F050C(void *);
extern void ov112_021F051C(void *);
extern void PokewalkerApp_Setup(void *);
extern void ov112_021EF31C(void *, void *);
extern void PokewalkerApp_Teardown(void *);
extern void ov112_021EF300(void *, void *, u32);
extern void ov112_021EF310(void *);
extern void ov112_021EF3F8(void);
extern void *sub_020932E0(int, int, int);
extern void sub_02093354(void *);
extern void sub_02093440(void *, void *, void *, void *, int, int, int, int, void *, void *, void *, void *, void *);
extern int sub_020935E0(void *);
extern void sub_02093A40(void *);
extern void sub_02093594(void *);

int ov112_021EEF30(void *p);
int ov112_021EEF68(void *p);
int ov112_021EF03C(void *p);
int ov112_021EF100(void *p);
void ov112_021EF15C(void);
void ov112_021EF17C(void *unused);
void ov112_021EF19C(void);
void ov112_021EF1CC(void *unused);
BOOL ov112_021EF1F0(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr);
BOOL ov112_021EF298(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr);

int ov112_021EF03C(void *p) {
    u8 *work = (u8 *)p + 0x1D764;

    switch (*(u32 *)work) {
    case 0:
        *(u32 *)work = 1;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, (enum HeapID)0x9A);
        *(u32 *)work = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *(u32 *)work = 3;
        }
        break;
    case 3: {
        int result = sub_020935E0(*(void **)(work + 4));
        if (result == 2) {
            *(u32 *)(work + 0x14) = 0;
            *(u32 *)work = 4;
        } else if (result == 3) {
            *(u32 *)(work + 0x14) = 1;
            *(u32 *)work = 4;
        }
        break;
    }
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, (enum HeapID)0x9A);
        *(u32 *)work = 5;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            ov112_021EF31C(work, p);
            return 3;
        }
        break;
    }

    sub_02093A40(*(void **)(work + 4));
    ov112_021F050C(*(void **)(work + 8));
    return 2;
}

int ov112_021EF100(void *p) {
    *(u32 *)((u8 *)p + 0x1D764) = 0;
    Main_SetVBlankIntrCB(NULL, NULL);
    sub_02093354(*(void **)((u8 *)p + 0x1D768));
    ov112_021F051C(*(void **)((u8 *)p + 0x1D76C));
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Heap_Free(*(void **)((u8 *)p + 0x18));
    PokewalkerApp_Setup(p);
    *(u32 *)((u8 *)p + 4) = *(u32 *)((u8 *)p + 0x1D778) == 1 ? 0 : 3;
    return 1;
}

void ov112_021EF15C(void) {
    GraphicsBanks banks = ov112_021FF0FC;
    GfGfx_SetBanks(&banks);
}

void ov112_021EF17C(void *unused) {
    GraphicsModes modes = ov112_021FF0DC;
    SetBothScreensModesAndDisable(&modes);
}

void ov112_021EF19C(void) {
    ObjCharTransferTemplate template = ov112_021FF0EC;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(0x14, (enum HeapID)0x9A);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void ov112_021EF1CC(void *unused) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, (enum HeapID)0x9A);
}

BOOL ov112_021EF1F0(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        ptr->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        ptr->personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        ptr->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        ptr->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        ptr->unkC = GetBoxMonData(boxMon, 6, NULL);
        ptr->unkE = 0;
        GetBoxMonData(boxMon, MON_DATA_NICKNAME, ptr->nickname);
        ptr->isShiny = BoxMonIsShiny(boxMon);
        ptr->gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL);
        ptr->unk14 = GetBoxMonData(boxMon, 0xA1, NULL);
        ptr->unk16 = GetBoxMonData(boxMon, MON_DATA_MARKINGS, NULL);
        return TRUE;
    }
    ptr->species = 0;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    return FALSE;
}

BOOL ov112_021EF298(PCStorage *storage, u32 boxno, u32 slotno, Ov112BoxMon *ptr) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        ptr->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        ptr->personality = 0;
        ptr->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        ptr->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        ptr->unkC = 0;
        ptr->unkE = 0;
        ptr->isShiny = 0;
        ptr->gender = 0;
        ptr->unk14 = 0;
        ptr->unk16 = 0;
        return TRUE;
    }
    ptr->species = 0;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    ptr->unk14 = 0;
    ptr->unk16 = 0;
    return FALSE;
}
